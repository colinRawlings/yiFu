// overseas the activities of the lens, interacts with the UI

#ifndef _LENS_MANAGER_H
#define _LENS_MANAGER_H

#include "UI.h"

class lensManager
{
    private:
      UI *ui;

      bool echo;

      int FD_plus;
      int FD_minus;

    public:
        // byte conversion functions

    public:

        int FD_set_plus_memory();
        int FD_set_minus_memory();

        int FD_recall_plus_memory();
        int FD_recall_minus_memory();

        void setEcho(bool echo_);

        //int Av_step_plus();
        //int Av_step_minus();

};

#endif // _LENS_MANAGER_H