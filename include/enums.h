#pragma once
#include <wx/defs.h>

enum sMenuID
{
   // File menu items
   sMENU_NEW = wxID_NEW,
   sMENU_OPEN = wxID_OPEN,
   sMENU_SAVE = wxID_SAVE,
   sMENU_SAVEAS = wxID_SAVEAS,
   sMENU_SAVEALL = 1,
   sMENU_CLOSE = wxID_CLOSE,
   sMENU_CLOSE_ALL = wxID_CLOSE_ALL,
   sMENU_RUN_SIM = 2,
   sMENU_EXIT = wxID_EXIT,

   // Edit menu items
   sMENU_REDO = wxID_REDO,
   sMENU_UNDO = wxID_UNDO,
   sMENU_ADDPROJ = 3,

   // View menu items
   sMENU_TOGTOOLBAR,
   sMENU_TOGSTATUSBAR,
   sMENU_ZOOMIN = wxID_ZOOM_IN,
   sMENU_ZOOMOUT = wxID_ZOOM_OUT,
   sMENU_ROTATE = 6,
   sMENU_TRANSLATE,
   sMENU_ROTANDTRANS,

   // Setting menu items
   sMENU_KEYBINDINGS,

   // Help menu items
   sMENU_ABOUT = wxID_ABOUT
};