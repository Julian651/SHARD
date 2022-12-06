#pragma once
#include <wx/wx.h>

enum shardMenuID
{
   // File menu items
   shardMENU_NEW = wxID_NEW,
   shardMENU_OPEN = wxID_OPEN,
   shardMENU_SAVE = wxID_SAVE,
   shardMENU_SAVEAS = wxID_SAVEAS,
   shardMENU_SAVEALL = 1,
   shardMENU_CLOSE = wxID_CLOSE,
   shardMENU_CLOSE_ALL = wxID_CLOSE_ALL,
   shardMENU_RUN_SIM = 2,
   shardMENU_EXIT = wxID_EXIT,

   // Edit menu items
   shardMENU_REDO = wxID_REDO,
   shardMENU_UNDO = wxID_UNDO,
   shardMENU_ADDPROJ = 3,

   // View menu items
   shardMENU_TOGTOOLBAR,
   shardMENU_TOGSTATUSBAR,
   shardMENU_ZOOMIN = wxID_ZOOM_IN,
   shardMENU_ZOOMOUT = wxID_ZOOM_OUT,
   shardMENU_ROTATE = 6,
   shardMENU_TRANSLATE,
   shardMENU_ROTANDTRANS,

   // Setting menu items
   shardMENU_KEYBINDINGS,

   // Help menu items
   shardMENU_ABOUT = wxID_ABOUT
};