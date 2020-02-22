/*
 * nokryptia : this program is a workaround for the encryption used
 *             in the mp3 player included with nokia5510 mobiles.
 *
 * Copyright (C) 2002 Roel Derickx <roel.derickx@campux.be>
 * Copyright (C) 2020 Charlene Wendling <julianaito@posteo.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <getopt.h>
#include <id3/id3lib_streams.h>
#include <id3/tag.h>

#define COPYBUFSIZE 10240

FILE * infile;
FILE * outfile;

char * infilename;
char * outfilename;
char * artistname;
char * songtitle;
int    verbose;
int    action;     /* 1 for upload, 2 for download */

void writeHelpMessage(const char * progname) {
	std::cout << PACKAGE << " version " << VERSION <<
	    " copyright by Roel Derickx and others\n"
	    "THIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY! "
	    "USE AT YOUR OWN RISK!\n"
	    "\nUsage:\n"
	    "   " << progname << " [options] file\n"
	    "\nOptions:\n"
	    "   -u               convert mp3 into lse for upload\n"
	    "   -d               convert lse into mp3 for download\n"
	    "   -a <artistname>  set artist to artistname\n"
	    "   -t <songtitle>   set title to songtitle\n"
	    "   -o <directory>   directory to where output files should be written\n"
	    "   -v               be verbose\n"
	    "   -h               print this help screen\n"
	    "\nSee the man page for more information\n";
}

/*
 * looknchange
 * This function looks for a pattern string2 in string1. If found,
 * string3 is placed in string1 on the place where string2 was found,
 * and the return value will be 0. If string2 was not found, return
 * value is non-zero.
 */
int looknchange(char * string1, char * string2, char * string3) {
	char *tmp;
	if (strlen(string1) < strlen(string2)) 
		return 1;
	tmp = string1+strlen(string1)-strlen(string2);
	if (strcasecmp(tmp, string2) != 0)
		return 1;
	strcpy(tmp, string3);
	return 0;
}

int parseInputFiles(int argc, char *argv[]){
	char * temp;

	/* look for .mp3 in infilename and change into .lse */
	/* or vice versa, depending on which action is chosen */
	/* also remove the path from infilename */

	if (optind >= argc) {
		std::cout << "No input file given!" << std::endl;
		return 1;
	}
	strcpy(infilename, argv[optind++]);
	if ((outfilename[strlen(outfilename)] == '/') ||
	    (strlen(outfilename) == 0)) {
		strcat(outfilename,"\0");
	}
	else {
		strcat(outfilename,"/\0");
	}
	temp = rindex(infilename,'/');
	if (temp != NULL)
		strcat(outfilename, temp+1);
	else
		strcat(outfilename, infilename);
  
  
	switch (action) {
	case 1:
		if (looknchange(outfilename, (char *) ".mp3", (char *) ".lse") != 0) {
			std::cout << "Input file must have mp3 "
			     << "extension!" << std::endl;
			return 1;
		}
		break;
	case 2:
		if (looknchange(outfilename, (char *) ".lse", (char *) ".mp3") != 0) {
			std::cout << "Input file must have lse "
			     << "extension!" << std::endl;
			return 1;
		}
		break;
	} /* end of switch */

	return 0;
}

int parseOptions(int argc, char * argv[]) {
	int c;
	
	/* first of all, check number of parameters */
	if (argc < 2) {
		return 1;
	}

	/* set default settings */
	strcpy(artistname,"unknown");
	strcpy(songtitle,"unknown");
	strcpy(infilename,"unknown");
	strcpy(outfilename,"\0");
	verbose = 0;
	action = 1; /* default action = upload */

	/* loop through all parameters */
	while ((c=getopt(argc,argv,"uda:t:hvo:"))!= -1 ){
		switch (c) {
		case 'h':
			writeHelpMessage(argv[0]);
			exit(0);
			break;
		case 'a':
			strncpy(artistname,optarg,30);
			strcat(artistname,"\0");
			break;
		case 't':
			strncpy(songtitle,optarg,30);
			strcat(songtitle,"\0");
			break;
		case 'v':
			verbose=1;
			break;
		case 'u':
			action=1;
			break;
		case 'd':
			action=2;
			break;
		case 'o':
			strcpy(outfilename,optarg);
			strcat(outfilename,"\0");
			break;
		default:
			std::cout << "Unknown option " << optopt << std::endl;
		} /* end of switch */
	} /* end of while */

	/* id3 tag support */
	ID3_Tag myTag;
	myTag.Link(infilename);
	ID3_Frame * myFrame;
	
	if ((strcmp(songtitle,"unknown") == 0) &&
	    (myFrame = myTag.Find(ID3FID_TITLE))) {
		myFrame->Field(ID3FN_TEXT).Get(songtitle,30);
		strcat(songtitle,"\0");
	}
	
	if ((strcmp(artistname,"unknown") == 0) &&
	    (myFrame = myTag.Find(ID3FID_LEADARTIST))) {
		myFrame->Field(ID3FN_TEXT).Get(artistname,30);
		strcat(artistname,"\0");
	}

	return parseInputFiles(argc,argv);
}

void makeTag(char * artist, char * title) {
	char * buf;
	int lengthArtist = strlen(artist);
	int lengthTitle  = strlen(title);

	if (verbose) {
		std::cout << "Writing tag to output :" << std::endl;
		std::cout << "artistname = " << artist << std::endl;
		std::cout << "songtitle  = " << title << std::endl;
	}

	/* allocate memory */
	buf = new char[384]();

	/* generate the data */
	memcpy(buf,"ID3",3);
	*(buf+3) = 0x03;
	*(buf+8) = 0x02;
	memcpy(buf+9,"vTIT2",5);
	*(buf+17) = lengthTitle+1;
	memcpy(buf+21,title,lengthTitle);
	memcpy(buf+21+lengthTitle,"TPE1",4);
	*(buf+28+lengthTitle) = lengthArtist+1;
	memcpy(buf+32+lengthTitle,artist, lengthArtist);
	memcpy(buf+32+lengthTitle+lengthArtist,"TLEN",4);
	*(buf+39+lengthTitle+lengthArtist) = 0x06;

	/* write the header to the file */
	fwrite(buf,384,1,outfile);

	/* clean up the memory */
	delete[] buf;
}

void makeHeader() {
	char * buf;

	if (verbose) std::cout << "Writing LockStream key to output" << std::endl;

	/* allocate memory */
	buf = new char[512]();

	/* generate the data */
	memcpy(buf,"LockStream Embedded\0d",21);
	*(buf+25) = 0x02;
	*(buf+32) = 0x02;
	*(buf+37) = 0x02;
	*(buf+104) = 0x02;
	*(buf+108) = 0x13;
	*(buf+112) = 0x80;

	/* write the header to the file */
	fwrite(buf,512,1,outfile);

	/* clean up the memory */
	delete[] buf;
}

int readAndTestHeader() {
	int bread, flag;
	char * buf;

	if (verbose) {
		std::cout << "Testing if the lse file is not written by "
		     << "the Nokia Audio Manager" << std::endl;
	}
	
	/* allocate memory */
	/* 384 bytes for the tag, 512 for the lockstream header */
	buf = new char[896];

	bread = fread(buf,1,896,infile);
	if (bread != 896) {
		std::cout << "Input file is too small!" << std::endl;
		return 0;
	}
	
	/* if this file is made by the official nokia program, */
	/* the key is not all zeroes and the file is thus encrypted */
	flag = 1;
	for (int i = 422; i < 526; i++) {
		if ((*(buf+i) != 0) &&
		    (i != 488) &&
		    (i != 492) &&
		    (i != 496)) flag = 0;
	}

	/* clean up the memory */
	delete[] buf;

	/* test is done, return value of flag */
	return flag;
}

void copyData() {
	int bread, bwritten;
	char * buf;

	if (verbose) std::cout << "Writing data to output" << std::endl;

	/* allocate memory */
	buf = new char[COPYBUFSIZE];
	
	/* copy the original mp3 into the lse */
	do {
		bread = fread(buf,1,COPYBUFSIZE,infile);
		bwritten = fwrite(buf,1,bread,outfile);
	} while (bread == bwritten && feof(infile) == 0);

	/* clean up the memory */
	delete[] buf;
}

void allocateMemory() {
	/* allocate memory for global variables */
	artistname = new char[31];
	songtitle = new char[31];
	infilename = new char[1000];
	outfilename = new char[1000];
}

void cleanUpMemory() {
	/* clean up memory */
	delete [] outfilename;
	delete [] infilename;
	delete [] songtitle;
	delete [] artistname;
}

int main(int argc, char * argv[]) {
	allocateMemory();

	/* parse the options */
	if (parseOptions(argc, argv) != 0) {
		writeHelpMessage(argv[0]);
		
		cleanUpMemory();
		
		/* exit with errorcode 1 */
		return 1;
	}
	
	/* open the infile */
	if ((infile = fopen(infilename,"rb")) == NULL) {
		std::cout << infilename
		     << ": No such file or directory" << std::endl;
		
		cleanUpMemory();
		
		/* exit with errorcode 1 */
		return 1;
	}
	
	/* open the outfile */
	if ((outfile = fopen(outfilename,"wb")) == NULL) {
		std::cout << "An error occured while opening "
		     << outfilename << " for writing." << std::endl
		     << "Do you have write permission "
		     << "in this directory?" << std::endl;
		
		cleanUpMemory();
		
		/* close the infile */
		fclose(infile);
		
		/* exit with errorcode 1 */
		return 1;
	}
	
	/* do the actual job */
	if (action == 1) {
		makeTag(artistname, songtitle);
		makeHeader();
		copyData();
	}
	else if (action == 2) {
		if (readAndTestHeader()) {
			copyData();
		}
		else {
			std::cout << "This file was encrypted by the "
			     << "official nokia program." << std::endl
			     << "Unable to decrypt!" << std::endl;
		}
	}
	
	/* close files */
	fclose(outfile);
	fclose(infile);

	cleanUpMemory();
	
	/* exit the program cleanly */
	return 0;
}

