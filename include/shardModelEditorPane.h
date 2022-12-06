#pragma once
#include "shardRenderPane.h"

class shardModelEditorPane : public shardRenderPane
{
private:

public:
   shardModelEditorPane(wxWindow* parent);
   virtual void Render(wxPaintEvent& evt) override;
};

