#pragma once
/*e
   *This class manages MOP files
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include "MopState.h"
#include "MopItem.h"
#include "Fragment.h"
#ifdef _WIN32
#include <zlib/zlib.h>
#else
#include <zlib.h>
#endif

#include <cstring>
#include <math.h>
class MopFile {
private:

inline double sqr( double x) const {
        return ((x)*(x));
}

/**
 * the output file stream
 */
std::ofstream outFileStream;

/**
 * the input file steam
 */
std::ifstream inFileStream;

/**
 * the mop file name to be used
 */
std::string filename;

/**
 * Unspecified error flag
 *
 */

int error;

/**
 * get a single character from the input file stream
 */
bool consumeChar(char &ch) {
        if (!this->inFileStream.eof()) {
                ch = this->inFileStream.get();
                return true;
        } else {
                return false;
        }
}
// Checks what the next value to be read is, used for checking for end of
// particles
char consumeCharPeek(char &ch) {
        if (!this->inFileStream.eof()) {
                ch = this->inFileStream.peek();
                return ch;
        } else {
                return '$';
        }
}
// Utility function to ignore characters for a certain length
bool consumeCharIgnore(char &ch, int length) {
        if (!this->inFileStream.eof()) {
                this->inFileStream.ignore(length, '$');
                return true;
        } else {
                return false;
        }
}

/**
 * read a MopState from the mop file and store it in a fragment
 * uses a std::string as an intermediate structure.
 *
 */
bool readMopStateFromFileAsText(Fragment &input, float skip) {
        std::string tmp;
        std::string tmp2;
        char ch;
        bool result = false;
        int count = 0;
        int particleLength;
        // Start by adding number of particles to string
        while (this->consumeCharPeek(ch) != '|') {
                result = this->consumeChar(ch);
                if (!result) return false;
                // std::cout << "Current Result is: " << ch << std::endl;
                tmp.push_back(ch);
        }
        do {
                result = this->consumeChar(ch);
                if (!result) return false;
                if (ch == '[') {
                        this->error = 1;
                        return false;
                }
                // Check for start of a particles
                else if (ch == '|') {
                        // Check whether to skip a paticle
                        if (count < skip) {
                                // Same as above but read rather than ignore
                                particleLength = this->getMopItemLength();
                                // std::cout << "> " << particleLength << std::endl;
                                for (int i = 0; i < particleLength + 1; i++) {
                                        result = this->consumeChar(ch);
                                        tmp.push_back(ch);
                                }
                                count++;
                        } else {
                                if(count == 10) {
                                        //std::cout << "Resseting Count" << std::endl;
                                        particleLength = this->getMopItemLength();
                                        // std::cout << "> " << particleLength << std::endl;
                                        for (int i = 0; i < particleLength + 1; i++) {
                                                result = this->consumeChar(ch);
                                                tmp.push_back(ch);
                                        }
                                        count = 0;
                                }
                                else{
                                        //result = this->consumeChar(ch);
                                        // Convert string to int and ignore characters to particle length
                                        particleLength = this->getMopItemLength();
                                        //  result = this->consumeChar(ch);
                                        result = this->consumeCharIgnore(ch, particleLength);
                                        result = this->consumeChar(ch);
                                        count++;
                                        // std::cout << "> " << particleLength << std::endl;
                                }

                        }
                } else
                        tmp.push_back(ch);

        } while (ch != '$');
        // Output contents of read string for debugging
        // std::cout << tmp;
        input.fill(tmp);
        // std::cout << "> Finished Reading State" << std::endl;
        return true;
}

int getMopItemLength() {
        std::string tmp;
        char ch;
        int len;
        while (this->consumeCharPeek(ch) != '|') {
                this->consumeChar(ch);
                tmp.push_back(ch);
        }
        len = atoi(tmp.c_str());
        // This could do with changing, without this if it ignores the last particle
        // It seems to skip past the end state indicator, causing seg fault
        return len -1;
}

/**
 * build a MopState from a fragment
 *
 */
MopState *buildMopStateFromFragment(Fragment &source, float skip) {
        MopState *ms = new MopState();
        Fragment worker;
        Fragment thing;
        int numParticles;
        int pos(0);
        pos = worker.fill(source, pos, '|');
        numParticles = worker.toInt();
        numParticles = round(numParticles * (skip / 10));
        //std::cout << "> Particle count " << numParticles << std::endl;

        for (int x(0); x < numParticles; x++) {
                MopItem mi;
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.id = thing.toInt();
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.size = thing.toInt();
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.r = thing.toInt();
                pos++;
		pos = thing.fill(source, pos, ',');
                mi.g = thing.toInt();
                pos++;
		pos = thing.fill(source, pos, ',');
                mi.b = thing.toInt();
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.x = thing.toFloat();
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.y = thing.toFloat();
                pos++;
                pos = thing.fill(source, pos, ',');
                mi.z = thing.toFloat();
                ms->addMopItem(mi);
                std::cout << "Loaded Object: " << x << " Out of: " << numParticles << std::endl;
        }
        // pstd::cout << "> converted the String to a MopState" << std::endl;
        return ms;
}

/**
 * if the filename given has no *.mop extension, add one.
 */
std::string setMopFileExtension(std::string fn) {
        std::string temp;
        if (!(fn.substr(fn.find_last_of(".") + 1) == "mop")) {
                temp = fn.append(".mop");
        }
        return temp;
}

/**
 * close the mopfile reader
 */
void closeMopfileReader() {
        this->inFileStream.close();
}

public:

MopFile() {
        this->error = 0;
}
~MopFile() {
}


/**
 * retrieve the filename
 */
std::string getFilename() {
        return this->filename;
}

/**
 * read a single state from a mopfile (mopfile must already be open)
 */
MopState *readState(float skipCount) {
        MopState *result = new MopState();
        //MopState *final = new MopState();
        // int itemCount;
        try {
                // start by reading in the next state from the file as a whole string
                Fragment initial;
                // std::cerr <<" > Reading the state as a Text String  " << std::endl;
                bool worked = this->readMopStateFromFileAsText(initial, skipCount);
                // std::cout <<initial.c_str()<<std::endl;

                if (!worked) {
                        return NULL;
                }

                // now convert that to a MopState object
                // std::cerr <<" > starting string to mopstate conversion" << std::endl;
                result = this->buildMopStateFromFragment(initial, skipCount);

                //final = this->rescale(result);
                // std::cerr <<" >finishing string to mopstate conversion" << std::endl;
                if (!result) {
                        return NULL;
                }
                if (this->error == 1) {
                        std::cerr << "Error reading state from Mopfile" << std::endl;
                        exit(0);
                }
        } catch (...) {
                std::cerr << "Error reading state from Mopfile" << std::endl;
                exit(0);
        }
        return result;
}

/*
 * read a single state from the mop file, cycling back to the start of the
 * file
 * if the end is reached
 */
MopState *readCyclingState(float skipCount) {
        // get the next state from the mopfile
        MopState *incoming = this->readState(skipCount);

        if (incoming == NULL) {
                this->resetFile();

                incoming = this->readState(skipCount);
        }
        return incoming;
}

/**
 * This method sets the filename Mopfile will use. This varient prepends the
 * path
 */
void setFilename(std::string path, std::string fn) {
        this->filename = path.append(this->setMopFileExtension(fn));
};

//This version is used by mopViewer, where it adds the Resources location and .mop extension
void setFilenameViewer(std::string fn) {
        std::string location = "Resources/";
        std::string extension = ".mop";
        this->filename = location + fn + extension;
};

 
/**
 * This writes the current set of particles to the mopfile.
 * and sends it to be written to the specified file.
 * The data are stored as follows:
 * 1: An integer declaring how many particles are in the state
 * 2: A set of MopItems in text form, each holding the data for a particle
 * Note: the trailing comma is required, as reading the elements back
 * correctly requires it
 */

 void writeState(std::string in) {
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        this->outFileStream << in;
        this->outFileStream.close();
    }


    
 	void writeState(MopState in) {
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        std::stringstream collect;
        std::stringstream write;
	std::stringstream finished;
        for (int x(0);x<in.getItemCount();x++) {
	  collect<<in.getMopItem(x).mopToStringStream();
        }
	write << collect.str();
        write<< "$";

        finished << in.getItemCount();
        finished << write.str();
	
        this->outFileStream <<finished.str();
	this->outFileStream.close();
    }

/**
 * close the mopfile writer
 */
void closeMopfileWriter() {
        this->outFileStream.close();
}
/**
 * This method sets the filename Mopfile
 * This method sets the filename Mopfile will use. This varient does not
 * prepend the path,
 * which assumes the path is already in the provided string, as would be the
 * case for a file open dialog.
 * The mop extention is also not added.
 */
void setFilename(std::string fn) {
        std::string location = "Resources/";
        std::string extension = ".mop";
        this->filename = location + fn + extension;
};

/**
 * open the mopfile for reading
 */
void openMopfileReader() {
        this->closeMopfileReader();
        try {
                this->inFileStream.open(this->filename.c_str(), std::ios_base::in);
                this->inFileStream.seekg(0, std::ios_base::beg);
        } catch (...) {
                std::cerr << "error: cannot open mop file" << std::endl;
        }
}

/**
 * reset the mop file back to the start of the file
 */
void resetFile() {
        // std::cout << "Resetting File: " << std::endl;
        this->closeMopfileReader();
        this->inFileStream.clear();
        this->openMopfileReader();
        this->inFileStream.seekg(0, std::ios_base::beg);
}




MopState *rescale(MopState *inputState) {
        MopState *displayState = new MopState();
        double m1,m2,m3;
        double maxDistanceFromOrigin;
        MopState *loadedState = inputState;

        MopItem tmp;
        int scaler;
        MopFile mopfile;
        bool firstState = true;
        scaler = 1;

        if (firstState) {
                firstState = false;
                // find out which particle is furthest out, on which axis
                double xv,yv,zv;
                double max = 0;
                for (int j(0); j<loadedState->getItemCount(); j++) {
                        xv = sqrt(sqr(loadedState->getMopItem(j).x));
                        yv = sqrt(sqr(loadedState->getMopItem(j).y));
                        zv = sqrt(sqr(loadedState->getMopItem(j).z));
                        if (xv>max) max = xv;
                        if (yv >max) max = yv;
                        if (zv>max) max = zv;
                }
                maxDistanceFromOrigin = max;
        }
	
        for (int x(0); x<loadedState->getItemCount(); x++) {
                m1 = sqrt(sqr(loadedState->getMopItem(x).x));
                m1 = m1 /(maxDistanceFromOrigin+(0.5*maxDistanceFromOrigin));
                std::cout << "M1: " << m1 << std::endl;
                m2 = (sqrt(sqr(loadedState->getMopItem(x).y))/(maxDistanceFromOrigin+(0.5*maxDistanceFromOrigin)));
                m3 = (sqrt(sqr(loadedState->getMopItem(x).z))/(maxDistanceFromOrigin+(0.5*maxDistanceFromOrigin)));
                if (loadedState->getMopItem(x).x>0) {
                        tmp.x = m1*scaler;
                        //std::cout << "Tmp X: " << tmp.x << std::endl;
                } else {
                        tmp.x = m1*(scaler);
                        //std::cout << "Tmp X: " << tmp.x << std::endl;
                }

                if (loadedState->getMopItem(x).y>0) {
                        tmp.y = m2*scaler;
                        //std::cout << "Tmp Y: " << tmp.y << std::endl;
                } else {
                        tmp.y = m2*(scaler);
                        //std::cout << "Tmp Y: " << tmp.y << std::endl;
                }

                if (loadedState->getMopItem(x).z>0) {
                        tmp.z = m3*scaler;
                        std::cout << "Tmp Z: " << tmp.z << std::endl;
                } else {
                        tmp.z = m3*(scaler);
                        //std::cout << "Tmp Z: " << tmp.z << std::endl;
                }
                // now colour
                tmp.r = loadedState->getMopItem(x).r;
                tmp.g = loadedState->getMopItem(x).g;
		tmp.b = loadedState->getMopItem(x).b;

                displayState->addMopItem(tmp);
        }

        //displayState->addMopItem(tmp);
        return displayState;
}

 /**
 * zlib compress and decompress functions:
 * Copyright 2007 Timo Bingmann <tb@panthema.net>
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 */
std::string compress (const std::string& str,
                            int compressionlevel = Z_BEST_COMPRESSION)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    deflateInit(&zs, compressionlevel);

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);


    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string decompress(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    inflateInit(&zs);

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    return outstring;
}

};
