/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


#ifndef NX_FRACTURE_TOOLS_H
#define NX_FRACTURE_TOOLS_H

#include "foundation/Px.h"
#include "NxExplicitHierarchicalMesh.h"
#include "NxFractureToolsStructs.h"

PX_PUSH_PACK_DEFAULT

namespace physx
{
namespace apex
{

struct IntersectMesh;
class NxDestructibleAsset;

class NxFractureTools
{
public:

	/** Instantiates a blank NxCutoutSet */
	virtual FractureTools::NxCutoutSet* createCutoutSet() = 0;

	/**
		Builds a cutout set (which must have been initially created by createCutoutSet()).
		Uses a bitmap described by pixelBuffer, bufferWidth, and bufferHeight.  Each pixel is represented
		by one byte in the buffer.

		\param cutoutSet		the NxCutoutSet to build
		\param pixelBuffer		pointer to be beginning of the pixel buffer
		\param bufferWidth		the width of the buffer in pixels
		\param bufferHeight		the height of the buffer in pixels
		\param snapThreshold	the pixel distance at which neighboring cutout vertices and segments may be fudged into alignment.
		\param periodic			whether or not to use periodic boundary conditions when creating cutouts from the map
	*/
	virtual void buildCutoutSet(FractureTools::NxCutoutSet& cutoutSet, const physx::PxU8* pixelBuffer, physx::PxU32 bufferWidth, physx::PxU32 bufferHeight, physx::PxF32 snapThreshold, bool periodic) = 0;

	/**
		Calculate the mapping between a cutout fracture map and a given triangle.
		The result is a 3 by 3 matrix M composed by an affine transformation and a rotation, we can get the 3-D projection for a texture coordinate pair (u,v) with such a formula:
		(x,y,z) = M*PxVec3(u,v,1)

		\param triangle		the target face's normal
		\param theMapping	resulted mapping, composed by an affine transformation and a rotation
	*/
	virtual bool calculateCutoutUVMapping(const physx::NxExplicitRenderTriangle& triangle, physx::PxMat33& theMapping) = 0;

	/**
		Uses the passed-in target direction to find the best triangle in the root mesh with normal near the given targetDirection.  If triangles exist
		with normals within one degree of the given target direction, then one with the greatest area of such triangles is used.  Otherwise, the triangle
		with normal closest to the given target direction is used.  The resulting triangle is used to calculate a UV mapping as in the function
		calculateCutoutUVMapping (above).

		The assumption is that there exists a single mapping for all triangles on a specified face, for this feature to be useful. 

		\param hMesh				the explicit mesh with well rectangle-shaped faces
		\param targetDirection		the target face's normal
		\param theMapping			resulted mapping, composed by an affine transformation and a rotation
	*/
	virtual bool calculateCutoutUVMapping(physx::NxExplicitHierarchicalMesh& hMesh, const physx::PxVec3& targetDirection, physx::PxMat33& theMapping) = 0;

	/**
		Splits the mesh in chunk[0], forming fractured pieces chunks[1...] using
		Voronoi decomposition fracturing.

		\param hMesh							the mesh to split
		\param iHMeshCore						if this mesh is not empty, chunk 0 will be used as an indestructible "core" of the fractured
													mesh.  That is, it will be subtracted from hMesh, and placed at level 1 of the hierarchy.  The remainder
													of hMesh will be split as usual, creating chunks at level 1 (and possibly deeper).
		\param exportCoreMesh					if true, a core mesh chunk will be created from iHMeshCore
		\param coreMeshImprintSubmeshIndex		if this is < 0, use the core mesh materials (was applyCoreMeshMaterialToNeighborChunks).  Otherwise, use the given submesh
		\param meshProcessingParams				describes generic mesh processing directives
		\param desc								describes the voronoi splitting parameters surfaces (see NxFractureVoronoiDesc)
		\param collisionDesc					convex hulls will be generated for each chunk using the method.  See NxCollisionDesc.
		\param randomSeed						seed for the random number generator, to ensure reproducibility.
		\param progressListener					The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param cancel							if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.
													Meant to be set from another thread.

		\return									true if successful.
	*/
	virtual bool	createVoronoiSplitMesh
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		physx::NxExplicitHierarchicalMesh& iHMeshCore,
		bool exportCoreMesh,
		physx::PxI32 coreMeshImprintSubmeshIndex,
		const FractureTools::NxMeshProcessingParameters& meshProcessingParams,
		const FractureTools::NxFractureVoronoiDesc& desc,
		const physx::NxCollisionDesc& collisionDesc,
		physx::PxU32 randomSeed,
		physx::IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Generates a set of uniformly distributed points in the interior of the root mesh.

		\param hMesh				the mesh in which to distribute sites
		\param siteBuffer			an array of PxVec3, at least the size of siteCount
		\param siteChunkIndices		if not NULL, then must be at least the size of siteCount.  siteCount indices will be written to this buffer, associating each site with a chunk that contains it.
		\param siteCount			the number of points to write into siteBuffer
		\param randomSeed			pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		\param microgridSize		pointer to a grid size used for BSP creation.  If NULL, the default settings will be used.
		\param meshMode				Open mesh handling.  Modes: Automatic, Closed, Open (see NxBSPOpenMode)
		\param progressListener		The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param chunkIndex			If this is a valid index, the voronoi sites will only be created within the volume of the indexed chunk.  Otherwise,
										the sites will be created within each of the root-level chunks.  Default value is an invalid index.

		\return						returns the number of sites actually created (written to siteBuffer and siteChunkIndices).  This may be less than the number of sites requested if site placement fails.
	*/
	virtual physx::PxU32	createVoronoiSitesInsideMesh
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		physx::PxVec3* siteBuffer,
		physx::PxU32* siteChunkIndices,
		physx::PxU32 siteCount,
		physx::PxU32* randomSeed,
		physx::PxU32* microgridSize,
		NxBSPOpenMode::Enum meshMode,
		physx::IProgressListener& progressListener,
		physx::PxU32 chunkIndex = 0xFFFFFFFF
	) = 0;

	/**
		Creates scatter mesh sites randomly distributed on the mesh.

		\param meshIndices						user-allocated array of size scatterMeshInstancesBufferSize which will be filled in by this function, giving the scatter mesh index used
		\param relativeTransforms				user-allocated array of size scatterMeshInstancesBufferSize which will be filled in by this function, giving the chunk-relative transform for each chunk instance
		\param chunkMeshStarts					user-allocated array which will be filled in with offsets into the meshIndices and relativeTransforms array.
													For a chunk indexed by i, the corresponding range [chunkMeshStart[i], chunkMeshStart[i+1]-1] in meshIndices and relativeTransforms is used.
													*NOTE*: chunkMeshStart array must be of at least size N+1, where N is the number of chunks in the base explicit hierarchical mesh.
		\param scatterMeshInstancesBufferSize	the size of meshIndices and relativeTransforms array.
		\param scatterMeshInstancesBufferSize	the size of meshIndices and relativeTransforms array.
		\param hMesh							the mesh in which to distribute sites
		\param targetChunkCount					how many chunks are in the array targetChunkIndices
		\param targetChunkIndices				an array of chunk indices which are candidates for scatter meshes.  The elements in the array chunkIndices will come from this array
		\param randomSeed						pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		\param scatterMeshAssetCount			the number of different scatter meshes (not instances).  Should not exceed 255.  If scatterMeshAssetCount > 255, only the first 255 will be used.
		\param scatterMeshAssets				an array of size scatterMeshAssetCount, of the render mesh assets which will be used for the scatter meshes
		\param minCount							an array of size scatterMeshAssetCount, giving the minimum number of instances to place for each mesh
		\param maxCount							an array of size scatterMeshAssetCount, giving the maximum number of instances to place for each mesh
		\param minScales						an array of size scatterMeshAssetCount, giving the minimum scale to apply to each scatter mesh
		\param maxScales						an array of size scatterMeshAssetCount, giving the maximum scale to apply to each scatter mesh
		\param maxAngles						an array of size scatterMeshAssetCount, giving a maximum deviation angle (in degrees) from the surface normal to apply to each scatter mesh

		return value							the number of instances placed in indices and relativeTransforms (will not exceed scatterMeshInstancesBufferSize)
	*/
	virtual physx::PxU32	createScatterMeshSites
	(
		physx::PxU8*						meshIndices,
		physx::PxMat44*						relativeTransforms,
		physx::PxU32*						chunkMeshStarts,
		physx::PxU32						scatterMeshInstancesBufferSize,
		physx::NxExplicitHierarchicalMesh&	hMesh,
		physx::PxU32						targetChunkCount,
		const physx::PxU16*					targetChunkIndices,
		physx::PxU32*						randomSeed,
		physx::PxU32						scatterMeshAssetCount,
		physx::NxRenderMeshAsset**			scatterMeshAssets,
		const physx::PxU32*					minCount,
		const physx::PxU32*					maxCount,
		const physx::PxF32*					minScales,
		const physx::PxF32*					maxScales,
		const physx::PxF32*					maxAngles
	) = 0;

	/**
		Utility to visualize Voronoi cells for a given set of sites.

		debugRender			rendering object which will receive the drawing primitives associated with this cell visualization
		sites				an array of Voronoi cell sites, of length siteCount
		siteCount			the number of Voronoi cell sites (length of sites array)
		cellColors			an optional array of colors (see NxApexRenderDebug for format) for the cells.  If NULL, the white (0xFFFFFFFF) color will be used.
								If not NULL, this (of length cellColorCount) is used to color the cell graphics.  The number cellColorCount need not match siteCount.  If
								cellColorCount is less than siteCount, the cell colors will cycle.  That is, site N gets cellColor[N%cellColorCount].
		cellColorCount		the number of cell colors (the length of cellColors array)
		bounds				defines an axis-aligned bounding box which clips the visualization, since some cells extend to infinity
		cellIndex			if this is a valid index (cellIndex < siteCount), then only the cell corresponding to sites[cellIndex] will be drawn.  Otherwise, all cells will be drawn.
	*/
	virtual void	visualizeVoronoiCells
	(
		physx::NxApexRenderDebug& debugRender,
		const physx::PxVec3* sites,
		physx::PxU32 siteCount,
		const physx::PxU32* cellColors,
		physx::PxU32 cellColorCount,
		const physx::PxBounds3& bounds,
		physx::PxU32 cellIndex = 0xFFFFFFFF
	) = 0;

	/**
		Builds a new NxExplicitHierarchicalMesh from an array of triangles.

		\param iHMesh					the NxExplicitHierarchicalMesh to build
		\param meshTriangles			pointer to array of NxExplicitRenderTriangles which make up the mesh
		\param meshTriangleCount		the size of the meshTriangles array
		\param submeshData				pointer to array of NxExplicitSubmeshData, describing the submeshes
		\param submeshCount				the size of the submeshData array
		\param meshPartition			if not NULL, an array of size meshPartitionCount, giving the end elements of contiguous subsets of meshTriangles.
										If meshPartition is NULL, one partition is assumed.
										When there is one partition, these triangles become the level 0 part.
										When there is more than one partition, these triangles become level 1 parts, the behavior is determined by firstPartitionIsDepthZero (see below).
		\param meshPartitionCount		if meshPartition is not NULL, this is the size of the meshPartition array.
		\param parentIndices			if not NULL, the parent indices for each chunk (corresponding to a partition in the mesh partition).
		\param parentIndexCount			the size of the parentIndices array.  This does not need to match meshPartitionCount.  If a mesh partition has an index beyond the end of parentIndices,
											then the parentIndex is considered to be 0.  Therefore, if parentIndexCount = 0, all parents are 0 and so all chunks created will be depth 1.  This will cause a
											depth 0 chunk to be created that is the aggregate of the depth 1 chunks.  If parentIndexCount > 0, then the depth-0 chunk must have a parentIndex of -1.
											To reproduce the effect of the old parameter 'firstPartitionIsDepthZero' = true, set parentIndices to the address of a PxI32 containing the value -1,
											and set parentIndexCount = 1.
											To reproduce the effect of the old parameter 'firstPartitionIsDepthZero' = false, set parentIndexCount = 0.
											Note: if parent indices are given, the first one must be -1, and *only* that index may be negative.
											That is, there may be only one depth-0 mesh and it must be the first mesh.
	*/
	virtual bool buildExplicitHierarchicalMesh
	(
		physx::NxExplicitHierarchicalMesh& iHMesh,
		const physx::NxExplicitRenderTriangle* meshTriangles,
		physx::PxU32 meshTriangleCount,
		const physx::NxExplicitSubmeshData* submeshData,
		physx::PxU32 submeshCount,
		physx::PxU32* meshPartition = NULL,
		physx::PxU32 meshPartitionCount = 0,
		physx::PxI32* parentIndices = NULL,
		physx::PxU32 parentIndexCount = 0
	) = 0;

	/**
		Set the tolerances used in CSG calculations with BSPs.

		\param linearTolerance		relative (to mesh size) tolerance used with angularTolerance to determine coplanarity.  Default = 1.0e-4.
		\param angularTolerance		used with linearTolerance to determine coplanarity.  Default = 1.0e-3
		\param baseTolerance		relative (to mesh size) tolerance used for spatial partitioning
		\param clipTolerance		relative (to mesh size) tolerance used when clipping triangles for CSG mesh building operations.  Default = 1.0e-4.
		\param cleaningTolerance	relative (to mesh size) tolerance used when cleaning the out put mesh generated from the toMesh() function.  Default = 1.0e-7.
	*/
	virtual void setBSPTolerances
	(
		physx::PxF32 linearTolerance,
		physx::PxF32 angularTolerance,
		physx::PxF32 baseTolerance,
		physx::PxF32 clipTolerance,
		physx::PxF32 cleaningTolerance
	) = 0;

	/**
		Set the parameters used in BSP building operations.

		\param logAreaSigmaThreshold	At each step in the tree building process, the surface with maximum triangle area is compared
											to the other surface triangle areas.  If the maximum area surface is far from the "typical" set of
											surface areas, then that surface is chosen as the next splitting plane.  Otherwise, a random
											test set is chosen and a winner determined based upon the weightings below.
											The value logAreaSigmaThreshold determines how "atypical" the maximum area surface must be to
											be chosen in this manner.
											Default value = 2.0.
		\param testSetSize				Larger values of testSetSize may find better BSP trees, but will take more time to create.
											testSetSize = 0 is treated as infinity (all surfaces will be tested for each branch).
											Default value = 10.
		\param splitWeight				How much to weigh the relative number of triangle splits when searching for a BSP surface.
											Default value = 0.5.
		\param imbalanceWeight			How much to weigh the relative triangle imbalance when searching for a BSP surface.
											Default value = 0.0.
	*/
	virtual void setBSPBuildParameters
	(
		physx::PxF32 logAreaSigmaThreshold,
		physx::PxU32 testSetSize,
		physx::PxF32 splitWeight,
		physx::PxF32 imbalanceWeight
	) = 0;


	/** 
		Builds the root NxExplicitHierarchicalMesh from an NxRenderMeshAsset.
		Since an NxDestructibleAsset contains no hierarchy information, the input mesh must have only one part.

		\param iHMesh			the NxExplicitHierarchicalMesh to build
		\param renderMeshAsset	input RenderMesh asset
		\param maxRootDepth		cap the root depth at this value.  Re-fracturing of the mesh will occur at this depth.  Default = PX_MAX_U32
	*/
	virtual bool buildExplicitHierarchicalMeshFromRenderMeshAsset(physx::NxExplicitHierarchicalMesh& iHMesh, const physx::NxRenderMeshAsset& renderMeshAsset, physx::PxU32 maxRootDepth = PX_MAX_U32) = 0;

	/** 
		Builds the root NxExplicitHierarchicalMesh from an NxDestructibleAsset.
		Since an NxDestructibleAsset contains hierarchy information, the explicit mesh formed
		will have this hierarchy structure.

		\param iHMesh				the NxExplicitHierarchicalMesh to build
		\param destructibleAsset	input Destructible asset
		\param maxRootDepth			cap the root depth at this value.  Re-fracturing of the mesh will occur at this depth.  Default = PX_MAX_U32
	*/
	virtual bool buildExplicitHierarchicalMeshFromDestructibleAsset(physx::NxExplicitHierarchicalMesh& iHMesh, const physx::NxDestructibleAsset& destructibleAsset, physx::PxU32 maxRootDepth = PX_MAX_U32) = 0;

	/**
		Splits the mesh in chunk[0], forming a hierarchy of fractured meshes in chunks[1...]

		\param hMesh							the mesh to split
		\param iHMeshCore						if this mesh is not empty, chunk 0 will be used as an indestructible "core" of the fractured
													mesh.  That is, it will be subtracted from hMesh, and placed at level 1 of the hierarchy.  The remainder
													of hMesh will be split as usual, creating chunks at level 1 (and possibly deeper).
		\param exportCoreMesh					if true, a core mesh chunk will be created from iHMeshCore
		\param coreMeshImprintSubmeshIndex		if this is < 0, use the core mesh materials (was applyCoreMeshMaterialToNeighborChunks).  Otherwise, use the given submesh
		\param meshProcessingParams				describes generic mesh processing directives
		\param desc								describes the slicing surfaces (see NxFractureSliceDesc)
		\param collisionDesc					convex hulls will be generated for each chunk using the method.  See NxCollisionDesc.
		\param randomSeed						seed for the random number generator, to ensure reproducibility.
		\param progressListener					The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param cancel							if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.
													Meant to be set from another thread.

		\return									returns true if successful.
	*/
	virtual bool createHierarchicallySplitMesh
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		physx::NxExplicitHierarchicalMesh& iHMeshCore,
		bool exportCoreMesh,
		physx::PxI32 coreMeshImprintSubmeshIndex,
		const FractureTools::NxMeshProcessingParameters& meshProcessingParams,
		const FractureTools::NxFractureSliceDesc& desc,
		const physx::NxCollisionDesc& collisionDesc,
		physx::PxU32 randomSeed,
		physx::IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Chips the mesh in chunk[0], forming a hierarchy of fractured meshes in chunks[1...]

		\param hMesh					the mesh to split
		\param meshProcessingParams		describes generic mesh processing directives
		\param desc						describes the slicing surfaces (see NxFractureCutoutDesc)
		\param iCutoutSet				the cutout set to use for fracturing (see NxCutoutSet)
		\param sliceDesc				used if desc.chunkFracturingMethod = SliceFractureCutoutChunks
		\param voronoiDesc				used if desc.chunkFracturingMethod = VoronoiFractureCutoutChunks
		\param collisionDesc			convex hulls will be generated for each chunk using the method.  See NxCollisionDesc.
		\param randomSeed				seed for the random number generator, to ensure reproducibility.
		\param progressListener			The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param cancel					if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.
											Meant to be set from another thread.

		\return							returns true if successful.
	*/
	virtual bool createChippedMesh
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		const FractureTools::NxMeshProcessingParameters& meshProcessingParams,
		const FractureTools::NxFractureCutoutDesc& desc,
		const FractureTools::NxCutoutSet& iCutoutSet,
		const FractureTools::NxFractureSliceDesc& sliceDesc,
		const FractureTools::NxFractureVoronoiDesc& voronoiDesc,
		const physx::NxCollisionDesc& collisionDesc,
		physx::PxU32 randomSeed,
		physx::IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Splits the chunk in chunk[chunkIndex], forming a hierarchy of fractured chunks using
		slice-mode fracturing.  The chunks will be rearranged so that they are in breadth-first order.

		\param hMesh					the ExplicitHierarchicalMesh to act upon
		\param chunkIndex				index of chunk to be split
		\param meshProcessingParams		used to create a BSP for this chunk
		\param desc						describes the slicing surfaces (see NxFractureSliceDesc)
		\param collisionDesc			convex hulls will be generated for each chunk using the method.  See NxCollisionDesc.
		\param randomSeed				pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		\param progressListener			The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param cancel					if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.
											Meant to be set from another thread.

		\return							returns true if successful.
	*/
	virtual bool hierarchicallySplitChunk
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		physx::PxU32 chunkIndex,
		const FractureTools::NxMeshProcessingParameters& meshProcessingParams,
		const FractureTools::NxFractureSliceDesc& desc,
		const physx::NxCollisionDesc& collisionDesc,
		physx::PxU32* randomSeed,
		physx::IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Splits the chunk in chunk[chunkIndex], forming fractured chunks using
		Voronoi decomposition fracturing.  The chunks will be rearranged so that they are in breadth-first order.

		\param hMesh					the ExplicitHierarchicalMesh to act upon
		\param chunkIndex				index of chunk to be split
		\param meshProcessingParams:	describes generic mesh processing directives
		\param desc						describes the voronoi splitting parameters surfaces (see NxFractureVoronoiDesc)
		\param collisionDesc			convex hulls will be generated for each chunk using the method.  See NxCollisionDesc.
		\param randomSeed				pointer to a seed for the random number generator, to ensure reproducibility.  If NULL, the random number generator will not be re-seeded.
		\param progressListener			The user must instantiate an IProgressListener, so that this function may report progress of this operation
		\param cancel					if not NULL and *cancel is set to true, the root mesh will be restored to its original state, and the function will return at its earliest opportunity.
											Meant to be set from another thread.

		\return							returns true if successful.
	*/
	virtual bool voronoiSplitChunk
	(
		physx::NxExplicitHierarchicalMesh& hMesh,
		physx::PxU32 chunkIndex,
		const FractureTools::NxMeshProcessingParameters& meshProcessingParams,
		const FractureTools::NxFractureVoronoiDesc& desc,
		const physx::NxCollisionDesc& collisionDesc,
		physx::PxU32* randomSeed,
		physx::IProgressListener& progressListener,
		volatile bool* cancel = NULL
	) = 0;

	/**
		Builds a mesh used for slice fracturing, given the noise parameters and random seed.  This function is mostly intended
		for visualization - to give the user a "typical" slice surface used for fracturing.
	*/
	virtual bool buildSliceMesh
	(
		physx::IntersectMesh& intersectMesh,
		physx::NxExplicitHierarchicalMesh& referenceMesh,
		const physx::PxPlane& slicePlane,
		const FractureTools::NxNoiseParameters& noiseParameters,
		physx::PxU32 randomSeed
	) = 0;

	/** Instantiates an NxExplicitHierarchicalMesh */
	virtual physx::NxExplicitHierarchicalMesh*	createExplicitHierarchicalMesh() = 0;

	/** Instantiates an NxExplicitHierarchicalMesh::NxConvexHull */
	virtual physx::NxExplicitHierarchicalMesh::NxConvexHull*	createExplicitHierarchicalMeshConvexHull() = 0;

};

}} // namespaces


PX_POP_PACK

#endif // NX_FRACTURE_TOOLS_H
