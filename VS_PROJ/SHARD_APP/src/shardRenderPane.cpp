#include "shardRenderPane.h"

shardRenderPane::shardRenderPane(wxWindow* parent) : wxGLCanvas(parent)
{
   this->SetName("GLCanvas");
   m_context = new wxGLContext(this);
   int argc = 1;
   char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
   glutInit(&argc, argv);
}

shardRenderPane::~shardRenderPane()
{
   delete m_context;
}

wxBEGIN_EVENT_TABLE(shardRenderPane, wxGLCanvas)
   EVT_PAINT(shardRenderPane::Render)
wxEND_EVENT_TABLE()