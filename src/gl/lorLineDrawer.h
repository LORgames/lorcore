#ifndef LOR_LINERENDERER
#define LOR_LINERENDERER

#include "lorMath.h"
#include "lorGraphicsCore.h"

struct lorLineDrawer;

void lorLineDrawer_Create(lorLineDrawer **ppLineDrawer, uint32_t maxLines = 2048);
void lorLineDrawer_Destroy(lorLineDrawer **ppLineDrawer);

bool lorLineDrawer_AddLine(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &x1, const float &y1, const uint32_t &colour);
bool lorLineDrawer_AddScreenRectangleAbsolute(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &x1, const float &y1, const uint32_t &colour);
bool lorLineDrawer_AddScreenRectangleRelative(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &width, const float &height, const uint32_t &colour);

void lorLineDrawer_Render(lorLineDrawer *pLineDrawer, lorGraphicsCore *pGL);

#endif
