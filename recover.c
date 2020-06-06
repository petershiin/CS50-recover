#include <stdio.h>

int main(int argc, char *argv[])
{
    //ensure two arguments
    if(argc != 2)
    {
        //print to error stream
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }

    //open card.raw for reading and assign pointer to it
    FILE *mem_card = fopen(argv[1], "r");

    //if something goes wrong
    if(mem_card == NULL)
    {
        //print to error stream
        fprintf(stderr, "File could not be opened");
        return 2;
    }

    //temp 512 byte storage //ensure everything is positive, could also use uint_8t here (a type of unsigned integer of length 8 bits)
    unsigned char buffer[512];
    //temp filename storage as we move through card.raw
    char filename[8];
    //pointer to a file
    FILE *img;
    int count = 0;

    //as long as fread returns 1 (as long as 512 bytes of data is read every time)
    //so anything less than 512 (i.e. EOF), we stop
    while(fread(buffer, 512, 1, mem_card))
    {
        //if header of jpg found
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer [2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //when not the first jpg found
            if(count > 0)
                fclose(img);

            //new file name
            sprintf(filename, "%03i.jpg", count);
            //open new file
            img = fopen(filename, "w");
            //write to new file
            fwrite(buffer, 512, 1, img);
            count++;
        }
        //write whenwver no new header is found, has condition of count > 0 so it only writes after the first header is found
        else if(count > 0)
            fwrite(buffer, 512, 1, img);
    }
    //close img since we're done
    fclose(img);
    //completion
    return 0;
}