import imvu
import md5
import logging
logger = logging.getLogger("imvu." + __name__)
import util.timing_samples

import ctypes

if __debug__:
    cal3d_dll  = ctypes.cdll.cal3d_debug
else:
    cal3d_dll = ctypes.cdll.cal3d

cal3d_dll.CalError_GetLastErrorText.restype = ctypes.c_char_p
cal3d_dll.CalCoreMaterial_GetMapFilename.restype = ctypes.c_char_p
cal3d_dll.CalCoreMaterial_GetMapType.restype = ctypes.c_char_p

gLoader = cal3d_dll.CalLoader_New()

gMaterialInfoCache = {}
gMeshInfoCache = {}

class Cal3dException(Exception): pass

def getMaterialInfo(materialBuffer):
    global gMaterialInfoCache
    cachekey = md5.md5(materialBuffer).hexdigest()
    cached = gMaterialInfoCache.get(cachekey, None)
    if cached: return cached

    global gLoader
    material = cal3d_dll.CalLoader_LoadCoreMaterialFromBuffer(gLoader, materialBuffer, len(materialBuffer))
    if not material:
        raise Cal3dException, cal3d_dll.CalError_GetLastErrorText()
    mapCount = cal3d_dll.CalCoreMaterial_GetMapCount(material)
    materialInfo = {}
    materialInfo['maps'] = {}
    for i in range(0, mapCount):
        mapType = cal3d_dll.CalCoreMaterial_GetMapType(material, i)
        mapName = cal3d_dll.CalCoreMaterial_GetMapFilename(material, i)
        materialInfo['maps'][mapType] = mapName
    cal3d_dll.CalCoreMaterial_Destroy(material)
    gMaterialInfoCache[cachekey] = materialInfo
    return materialInfo
getMaterialInfo = util.timing_samples.profiledFunction(getMaterialInfo)

def getMeshInfo(meshBuffer):
    global gMeshInfoCache
    cachekey = md5.md5(meshBuffer).hexdigest()
    cached = gMeshInfoCache.get(cachekey, None)
    if cached:
        return cached

    global gLoader
    mesh = cal3d_dll.CalLoader_LoadCoreMeshFromBuffer(gLoader, meshBuffer, len(meshBuffer))
    if not mesh:
        raise Cal3dException, cal3d_dll.CalError_GetLastErrorText()
    submeshCount = cal3d_dll.CalCoreMesh_GetCoreSubmeshCount(mesh)
    result = {}
    for i in range(0, submeshCount):
        result[i] = {}
        submesh = cal3d_dll.CalCoreMesh_GetCoreSubmesh(mesh, i)
        result[i]['materialId'] = cal3d_dll.CalCoreSubmesh_GetCoreMaterialThreadId(submesh)
        result[i]['faceCount'] = cal3d_dll.CalCoreSubmesh_GetFaceCount(submesh)
        result[i]['lodCount'] = cal3d_dll.CalCoreSubmesh_GetLodCount(submesh)
        result[i]['springCount'] = cal3d_dll.CalCoreSubmesh_GetSpringCount(submesh)
        result[i]['vertexCount'] = cal3d_dll.CalCoreSubmesh_GetVertexCount(submesh)
        result[i]['tangentsEnabled'] = cal3d_dll.CalCoreSubmesh_IsTangentsEnabled(submesh)
    cal3d_dll.CalCoreMesh_Destroy(mesh)
    gMeshInfoCache[cachekey] = result
    return result
getMeshInfo = util.timing_samples.profiledFunction(getMeshInfo)