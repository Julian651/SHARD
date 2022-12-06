#include "toolbar.h"
#include <shard.h>
#include <wx/artprov.h>
Toolbar::Toolbar(wxWindow * parent)
    : wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4L | wxBORDER_NONE) {

    wxImage::AddHandler(new wxPNGHandler);

   
 
   

    this->AddTool(wxID_EXIT, wxT("Quit"), wxArtProvider::GetBitmap("wxART_QUIT"));
    this->AddTool(shardMENU_NEW, wxT("New"), wxArtProvider::GetBitmap("wxART_NEW"));
    this->AddTool(wxID_ANY, wxT("Open"), wxArtProvider::GetBitmap("wxART_FILE_OPEN"));
    this->AddTool(wxID_ANY, wxT("Save"), wxArtProvider::GetBitmap("wxART_FILE_SAVE"));
    this->AddTool(wxID_ANY, wxT("Save As"), wxArtProvider::GetBitmap("wxART_FILE_SAVE_AS"));
    this->AddTool(wxID_HELP, wxT("Help"), wxArtProvider::GetBitmap("wxART_HELP"));
    this->AddTool(wxID_ANY, wxT("Open"), wxArtProvider::GetBitmap("wxART_FILE_OPEN"));
    this->AddTool(wxID_ANY, wxT("Forward"), wxArtProvider::GetBitmap("wxART_GO_FORWARD"));
    this->AddTool(wxID_ANY, wxT("Backward"), wxArtProvider::GetBitmap("wxART_GO_BACK"));
    this->AddTool(wxID_ANY, wxT("Zoom In"), wxArtProvider::GetBitmap("wxART_PLUS"));
    this->AddTool(wxID_ANY, wxT("Zoom Out"), wxArtProvider::GetBitmap("wxART_MINUS"));
    this->AddTool(wxID_ANY, wxT("Add Tile"), wxArtProvider::GetBitmap("wxART_ADD_BOOKMARK"));
    this->AddTool(wxID_ANY, wxT("Delete Tile"), wxArtProvider::GetBitmap("wxART_DEL_BOOKMARK"));
    

    this->Realize();
}

