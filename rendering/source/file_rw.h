//
//  file_rw.h
//  tau3d
//
//  Created by Carl Johan Gribel on 2015-01-11.
//
//

#ifndef tau3d_file_rw_h
#define tau3d_file_rw_h

/**
 * mode e.g. "rb" (binary), "rt" (text)
 */
static char* read_file(const char *filename, const char *mode)
{
    FILE *fp;
    char *content = NULL;
    
    long count=0;
    
    if (filename != NULL) {
        fp = fopen(filename, mode);
        
        if (fp != NULL) {
            
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        } else {
            printf("EXITING : couldn't load file %s\n",filename);
            exit(77);
        }
    }
    return content;
}

static char* read_binary_file(const char *filename) { return read_file(filename, "rb"); }

static char* read_text_file(const char *filename) { return read_file(filename, "rt"); }

/**
 * mode e.g. "wb" (binary), "wt" (text)
 */
static int file_write(char *fn, char *s, const char *mode)
{
    FILE *fp;
    int status = 0;
    
    if (fn != NULL) {
        fp = fopen(fn,"wb");
        
        if (fp != NULL) {
            
            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }
    return(status);
}

#endif
