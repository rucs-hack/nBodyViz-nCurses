#pragma once
#include "mopItem.h"
std::string mopToString(mopItem);
#ifdef MOPFILE
std::string mopToString(mopItem in) {
  std::stringstream tmp;
  std::stringstream write;
  tmp << in.name << ",";
  tmp << in.mass << ",";
  tmp << in.radius << ",";
  tmp << in.size<< ",";
  tmp << in.r << ",";
  tmp << in.g << ",";
  tmp << in.b << ",";
  tmp << in.x << ",";
  tmp << in.y << ",";
  tmp << in.z << ",";
  tmp << in.xd << ",";
  tmp << in.yd << ",";
  tmp << in.zd << ",";
  write << "|";
  write << tmp.str().length()+1;
  write << "|";
  write << tmp.str();
  return write.str();
}
#endif
#ifdef MOPFILELIGHT
std::string mopToString(mopItem in) {
  std::stringstream tmp;
  std::stringstream write;
  tmp << in.id << ",";
  tmp << in.size << ",";
  tmp << in.r << ",";
  tmp << in.g << ",";
  tmp << in.b << ",";
  tmp << in.x << ",";
  tmp << in.y << ",";
  tmp << in.z << ",";
  write << "|";
  write << tmp.str().length()+1;
  write << "|";
  write << tmp.str();     
  return write.str();
}
#endif

