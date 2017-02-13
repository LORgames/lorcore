#include "lorLineDrawer.h"
#include "lorCore.h"
#include "lorGL_Internal.h"

struct lorLineVertex
{
  float Position[2];
  uint8_t Colour[3];
};

struct lorLineDrawer
{
  //Variables
  int32_t usedLines;
  int32_t maxLines;

  lorLineVertex* vertices;
  int32_t totalVertices;

  bool hasChanged;
};

void lorLineDrawer_Create(lorLineDrawer **ppLineDrawer, uint32_t maxLines)
{
  *ppLineDrawer = lorAllocType(lorLineDrawer, 1);
  (*ppLineDrawer)->maxLines = maxLines;

  (*ppLineDrawer)->vertices = lorAllocType(lorLineVertex, maxLines * 2);
  (*ppLineDrawer)->totalVertices = maxLines * 2;
  (*ppLineDrawer)->usedLines = 0;
}

void lorLineDrawer_Destroy(lorLineDrawer **ppLineDrawer)
{
  lorFree((*ppLineDrawer)->vertices);
  lorFree(*ppLineDrawer);

  *ppLineDrawer = nullptr;
}

bool lorLineDrawer_AddLine(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &x1, const float &y1, const uint32_t &colour)
{
  if (pLineDrawer->usedLines == pLineDrawer->maxLines)
    return false;

  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 0].Position[0] = x0;
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 0].Position[1] = y0;
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 0].Colour[0] = ((colour >> 16) & 0xFF);
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 0].Colour[1] = ((colour >> 8) & 0xFF);
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 0].Colour[2] = (colour & 0xFF);

  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 1].Position[0] = x1;
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 1].Position[1] = y1;
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 1].Colour[0] = ((colour >> 16) & 0xFF);
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 1].Colour[1] = ((colour >> 8) & 0xFF);
  pLineDrawer->vertices[pLineDrawer->usedLines * 2 + 1].Colour[2] = (colour & 0xFF);

  pLineDrawer->hasChanged = true;
  pLineDrawer->usedLines++;

  return true;
}

bool lorLineDrawer_AddScreenRectangleAbsolute(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &x1, const float &y1, const uint32_t &colour)
{
  bool retVal = true;

  retVal &= lorLineDrawer_AddLine(pLineDrawer, x0, y0, x0, y1, colour); //Left line
  retVal &= lorLineDrawer_AddLine(pLineDrawer, x1, y0, x1, y1, colour); //Right line
  retVal &= lorLineDrawer_AddLine(pLineDrawer, x0, y0, x1, y0, colour); //Top line
  retVal &= lorLineDrawer_AddLine(pLineDrawer, x0, y1, x1, y1, colour); //Bottom line

  return retVal;
}

bool lorLineDrawer_AddScreenRectangleRelative(lorLineDrawer *pLineDrawer, const float &x0, const float &y0, const float &width, const float &height, const uint32_t &colour)
{
  return lorLineDrawer_AddScreenRectangleAbsolute(pLineDrawer, x0, y0, x0 + width, y0 + height, colour);
}

void lorLineDrawer_Render(lorLineDrawer *pLineDrawer, lorGraphicsCore *pGL)
{
  lorLineVertex *p0, *p1;

  for (int i = 0; i < pLineDrawer->usedLines; i++)
  {
    p0 = &pLineDrawer->vertices[i * 2];
    p1 = &pLineDrawer->vertices[i * 2 + 1];
    SDL_SetRenderDrawColor(pGL->pRenderer, p0->Colour[0], p0->Colour[1], p0->Colour[2], 0xFF);
    SDL_RenderDrawLine(pGL->pRenderer, (int)p0->Position[0], (int)p0->Position[1], (int)p1->Position[0], (int)p1->Position[1]);
  }

  pLineDrawer->usedLines = 0;
}
