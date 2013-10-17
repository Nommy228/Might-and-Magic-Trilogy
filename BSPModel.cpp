#include <stdlib.h>

#include "BSPModel.h"



//----- (00478389) --------------------------------------------------------
void BSPModel::Release()
{
  BSPModel *v1; // esi@1
  void *v2; // ST00_4@1
  void *v3; // ST00_4@1
  void *v4; // ST00_4@1

  v1 = this;
  free(*(void **)&this->pVertices.pVertices);
  v2 = v1->pFaces;
  v1->pVertices.pVertices = 0;
  free(v2);
  v3 = v1->pFacesOrdering;
  v1->pFaces = 0;
  free(v3);
  v4 = v1->pNodes;
  v1->pFacesOrdering = 0;
  free(v4);
  v1->pNodes = 0;
  v1->uNumNodes = 0;
  v1->uNumFaces = 0;
  v1->pVertices.uNumVertices = 0;
  v1->uNumConvexFaces = 0;
}
