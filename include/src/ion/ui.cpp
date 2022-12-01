#include "ion/ui.hpp"

namespace ImGui {
void TableSetupColumn(ColumnView const & params)
{
    TableSetupColumn(params.id.c_str(), params.flags, params.init_width);
}

bool NewWindow(WindowView const & params)
{
    SetNextWindowPos(params.position, ImGuiCond_Once);
    SetNextWindowSize(params.size, ImGuiCond_Once);
    return Begin(params.id.c_str(), params.is_open, params.flags);
}
}
