/**
* This class is used to store a single MOP file entry, which represents the state of a particle, in abbreviated form for larger file sizes
*/
#pragma once
#include <string>
/**
* This simple class represents a single particle recorded to the mopfile.
*/
class MopItem
{
public:
    MopItem() {
    }

    ~MopItem() {
    }
    
#ifdef MOPFILE
    float mass;
    float radius;
    //position in the WCS
    float x;
    float y;
    float z;
    //Velocity in the WCS
    float xv;
    float yv;
    float zv;
    // the colour of a particle
    int r;
    int g;
    int b;
    std::string name; // name of particle
    int type;   // used to tell this particle what type it is (collapsor/normal particle/placemark)
    int size; // what shape/size this particle will have when displayed in a visualisation

        void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->mass = other.mass;
            this->radius = other.radius;
            this->id = other.id;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->xv = other.xv;
            this->yv = other.yv;
            this->zv = other.zv;
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            this->name = other.name;
            this->type = other.type;
            this->size = other.size;
        }
    }

    
    void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->mass = other.mass;
            this->radius = other.radius;
            this->id = other.id;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->xv = other.xv;
            this->yv = other.yv;
            this->zv = other.zv;
            this->r = other.r;
	    this->g = other.g;
	    this->b = other.b;
            this->name = other.name;
            this->size = other.size;
        }
    }

    std::stringstream mopToStringStream() {
        std::stringstream tmp;
        std::stringstream write;

            tmp << this->name << ",";
            tmp << this->mass << ",";
            tmp << this->radius << ",";
            tmp << this->size<< ",";
	    tmp << this->r << ",";
	    tmp << this->g << ",";
	    tmp << this->b << ",";
            tmp << this->x << ",";
            tmp << this->y << ",";
            tmp << this->z << ",";
	    tmp << this->xd << ",";
            tmp << this->yd << ",";
            tmp << this->zd << ",";
            write << "|";
	    write << tmp.str().length()+1;
            write << "|";
            write << tmp.str();
	    //std::cout <<"this" << write << std::endl;

        return write.str();
    }
    
#endif


#ifdef MOPFILELIGHT
    int id;
    int size; // what shape/size this particle will have when displayed in a visualisation
    // the colour of a particle
    int r;
    int g;
    int b;
    //position in the WCS
    double x;
    double y;
    double z;
    
    void fillFromExistingMopItem(const MopItem & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->id = other.id;
            this->size = other.size;
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }
    }

    std::string mopToStringStream() {
        std::stringstream tmp;
        std::stringstream write;

            tmp << this->id << ",";
            tmp << this->size << ",";
	    tmp << this->r << ",";
	    tmp << this->g << ",";
	    tmp << this->b << ",";
            tmp << this->x << ",";
            tmp << this->y << ",";
            tmp << this->z << ",";
            write << "|";
	    write << tmp.str().length()+1;
            write << "|";
            write << tmp.str();     
        return write.str();
	
    }

#endif

};
