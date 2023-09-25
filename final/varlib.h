int VLstore(char* name, char * val);
int VLdelete(char* name);
char *VLlookup(char* name);
int VLexport(char* name);
void VLlist();
int VLenviron2table(char* env[]);
char** VLtable2environ();
int VLign(char* signal);
int VLdef(char* signal);