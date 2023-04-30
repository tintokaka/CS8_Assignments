#include "file_record.h"

long FileRecord::write(fstream &outs)
{
       outs.seekg(0, outs.end);
        long pos = outs.tellp();
        for (char* record : _records) {
            outs.write(record, MAX+1);
        }
        return pos / (_records.size() * (MAX+1));
}

long FileRecord::read(fstream &ins, long recno)
{   
     long bytesRead = 0;
    for (char* record : _records) {
        if (!ins.read(record, MAX+1)) {  // If read fails
            if (ins.eof()) {
                break;  // Stop reading at the end of file
            } else {
                // Error handling code here
                return -1;
            }
        }
        bytesRead += ins.gcount();
        if (*record == '\0') {
            break;
        }
    }
    return bytesRead;
}
