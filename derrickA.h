#ifndef _DERRICKS_H_
#define _DERRICKS_H_

class Level
{
    public:
        Shape box[7];
        int xres, yres;
        Shape s;
        Vec position;
        bool activated;
        int levelID;

        Level() {
                xres = 1200;
                yres = 900;
                levelID = 1;
        }
};


#endif //_DERRICKS_H_
