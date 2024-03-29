#include "sdCardOps.h"

bool listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("* > Listing directory: %s\n\r", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("* > Failed to open directory.");
    return false;
  }
  if(!root.isDirectory()){
    Serial.println("* > Not a directory.");
    return false;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  return true;
}

bool createDir(fs::FS &fs, const char * path){
  Serial.printf("* > Creating Dir: %s\n\r", path);
  if(fs.mkdir(path)){
    Serial.println("* > Dir created.");
    return true;
  } else {
    Serial.println("* > mkdir failed.");
    return false;
  }
}

bool removeDir(fs::FS &fs, const char * path){
  Serial.printf("* > Removing Dir: %s\n\r", path);
  if(fs.rmdir(path)){
    Serial.println("* > Dir removed");
    return true;
  } else {
    Serial.println("* > rmdir failed");
    return false;
  }
}

bool readFile(fs::FS &fs, const char * path){
  Serial.printf("* > Reading file: %s\n\r", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("* > Failed to open file for reading");
    return false;
  }

  Serial.print("* > Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
  return true;
}

bool writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("* > Writing file: %s\n\r", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("* > Failed to open file for writing");
    return false;
  }
  if(file.print(message)){
    Serial.println("* > File written");
    file.close();
    return true;
  } else {
    Serial.println("* > Write failed");
    file.close();
    return false;
  }
}

bool appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("* > Appending to file: %s\n\r", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("* > Failed to open file for appending");
    return false;
  }
  if(file.print(message)){
      Serial.println("* > Message appended");
      file.close();
      return true;
  } else {
    Serial.println("* > Append failed");
    file.close();
    return false;
  }
}

bool renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("* > Renaming file %s to %s\n\r", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("* > File renamed");
    return true;
  } else {
    Serial.println("* > Rename failed");
    return false;
  }
}

bool deleteFile(fs::FS &fs, const char * path){
  Serial.printf("* > Deleting file: %s\n\r", path);
  if(fs.remove(path)){
    Serial.println("* > File deleted");
    return true;
  } else {
    Serial.println("* > Delete failed");
    return false;
  }
}

bool testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n\r", flen, end);
    file.close();
  } else {
    Serial.println("* > Failed to open file for reading");
    return false;
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return false;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n\r", 2048 * 512, end);
  file.close();
  return true;
}