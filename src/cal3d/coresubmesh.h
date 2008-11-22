//****************************************************************************//
// coresubmesh.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include <boost/shared_ptr.hpp>
#include "cal3d/global.h"
#include "cal3d/vector.h"

class CalCoreSubMorphTarget;


enum CalMorphTargetType {
  CalMorphTargetTypeNull = 0,
  CalMorphTargetTypeAdditive,
  CalMorphTargetTypeClamped,
  CalMorphTargetTypeAverage,
  CalMorphTargetTypeExclusive
};

class CAL3D_API CalCoreSubmesh
{
public:
  /// The core submesh TextureCoordinate.
  struct TextureCoordinate
  {
    float u, v;
  };

  struct TangentSpace
  {
    CalVector tangent;
    float crossFactor;  // To get the binormal, use ((N x T) * crossFactor)
  };


  /// The core submesh Influence.
  struct Influence
  {
    int boneId;
    float weight;
  };

  /// The core submesh Vertex.
  struct Vertex
  {
    CalVector position;
    CalVector normal;
    std::vector<Influence> vectorInfluence;
    int collapseId;
    int faceCollapseCount;
    CalVector vertexColor; 
  };

  struct Face {
      CalIndex vertexId[3];
  };
  
  typedef std::vector<boost::shared_ptr<CalCoreSubMorphTarget> > CoreSubMorphTargetVector;
  typedef std::vector<Face> VectorFace;
  typedef std::vector<TangentSpace> VectorTangentSpace;
  typedef std::vector<TextureCoordinate> VectorTextureCoordinate;
  typedef std::vector<VectorTangentSpace > VectorVectorTangentSpace;
  typedef std::vector<VectorTextureCoordinate > VectorVectorTextureCoordinate;
  typedef std::vector<Vertex> VectorVertex;
  typedef std::vector<Influence> VectorInfluence;

  CalCoreSubmesh();

  unsigned int size();
  unsigned int sizeWithoutSubMorphTargets();
  int getCoreMaterialThreadId();
  int getFaceCount();
  int getLodCount();
  bool hasNonWhiteVertexColors() { return m_hasNonWhiteVertexColors; }
  std::vector<Face>& getVectorFace();
  std::vector<std::vector<TangentSpace> >& getVectorVectorTangentSpace();
  std::vector<std::vector<TextureCoordinate> >& getVectorVectorTextureCoordinate();
  std::vector<Vertex>& getVectorVertex();
  int getVertexCount();
  bool isTangentsEnabled(int mapId);
  bool enableTangents(int mapId, bool enabled);
  void reserve(int vertexCount, int textureCoordinateCount, int faceCount);
  void setCoreMaterialThreadId(int coreMaterialThreadId);
  bool setFace(int faceId, const Face& face);
  void setLodCount(int lodCount);
  bool setTangentSpace(int vertexId, int textureCoordinateId, const CalVector& tangent, float crossFactor);
  bool setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate);
  bool setVertex(int vertexId, const Vertex& vertex);
  void setHasNonWhiteVertexColors( bool p ) { m_hasNonWhiteVertexColors = p; }
  int addCoreSubMorphTarget(boost::shared_ptr<CalCoreSubMorphTarget> pCoreSubMorphTarget);
  boost::shared_ptr<CalCoreSubMorphTarget> getCoreSubMorphTarget(int id);
  int getCoreSubMorphTargetCount();
  CoreSubMorphTargetVector& getVectorCoreSubMorphTarget();
  void scale(float factor);
  void setSubMorphTargetGroupIndexArray( unsigned int len, unsigned int const * indexArray );
  inline unsigned int subMorphTargetGroupIndex( int subMorphTargetId ) { 
    if( size_t(subMorphTargetId) >= m_vectorSubMorphTargetGroupIndex.size() ) return 0xffffffff;
    return m_vectorSubMorphTargetGroupIndex[ subMorphTargetId ];
  }

private:
  void UpdateTangentVector(int v0, int v1, int v2, int channel);

  std::vector<Vertex> m_vectorVertex;
  std::vector<bool> m_vectorTangentsEnabled;
  std::vector<std::vector<TangentSpace> > m_vectorvectorTangentSpace;
  std::vector<std::vector<TextureCoordinate> > m_vectorvectorTextureCoordinate;
  std::vector<Face> m_vectorFace;
  CoreSubMorphTargetVector m_vectorCoreSubMorphTarget;
  int m_coreMaterialThreadId;
  int m_lodCount;
  std::vector<unsigned int> m_vectorSubMorphTargetGroupIndex;
  bool m_hasNonWhiteVertexColors;
};
