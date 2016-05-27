/*
 * Copyright 2011-2016 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __UTIL_TEXTURE_H__
#define __UTIL_TEXTURE_H__

CCL_NAMESPACE_BEGIN

/* Texture limits on devices. */

/* CPU */
#define TEX_NUM_FLOAT4_CPU		1024
#define TEX_NUM_BYTE4_CPU		1024
#define TEX_NUM_FLOAT_CPU		1024
#define TEX_NUM_BYTE_CPU		1024
#define TEX_START_BYTE4_CPU		TEX_NUM_FLOAT4_CPU
#define TEX_START_FLOAT_CPU		(TEX_NUM_FLOAT4_CPU + TEX_NUM_BYTE4_CPU)
#define TEX_START_BYTE_CPU		(TEX_NUM_FLOAT4_CPU + TEX_NUM_BYTE4_CPU + TEX_NUM_BYTE_CPU)

/* CUDA (Geforce 4xx and 5xx) */
#define TEX_NUM_FLOAT4_CUDA		5
#define TEX_NUM_BYTE4_CUDA		88
#define TEX_NUM_FLOAT_CUDA		0
#define TEX_NUM_BYTE_CUDA		0
#define TEX_START_BYTE4_CUDA	TEX_NUM_FLOAT4_CUDA
#define TEX_START_FLOAT_CUDA	(TEX_NUM_FLOAT4_CUDA + TEX_NUM_BYTE4_CUDA)
#define TEX_START_BYTE_CUDA		(TEX_NUM_FLOAT4_CUDA + TEX_NUM_BYTE4_CUDA + TEX_NUM_BYTE_CUDA)

/* CUDA (Kepler, Geforce 6xx and above) */
#define TEX_NUM_FLOAT4_CUDA_KEPLER		1024
#define TEX_NUM_BYTE4_CUDA_KEPLER		1024
#define TEX_NUM_FLOAT_CUDA_KEPLER		1024
#define TEX_NUM_BYTE_CUDA_KEPLER		1024
#define TEX_START_BYTE4_CUDA_KEPLER		TEX_NUM_FLOAT4_CUDA_KEPLER
#define TEX_START_FLOAT_CUDA_KEPLER		(TEX_NUM_FLOAT4_CUDA_KEPLER + TEX_NUM_BYTE4_CUDA_KEPLER)
#define TEX_START_BYTE_CUDA_KEPLER		(TEX_NUM_FLOAT4_CUDA_KEPLER + TEX_NUM_BYTE4_CUDA_KEPLER + TEX_NUM_BYTE_CUDA_KEPLER)

/* OpenCL */
#define TEX_NUM_FLOAT4_OPENCL	1024
#define TEX_NUM_BYTE4_OPENCL	1024
#define TEX_NUM_FLOAT_OPENCL	0
#define TEX_NUM_BYTE_OPENCL		0
#define TEX_START_BYTE4_OPENCL	TEX_NUM_FLOAT4_OPENCL
#define TEX_START_FLOAT_OPENCL	(TEX_NUM_FLOAT4_OPENCL + TEX_NUM_BYTE4_OPENCL)
#define TEX_START_BYTE_OPENCL	(TEX_NUM_FLOAT4_OPENCL + TEX_NUM_BYTE4_OPENCL + TEX_NUM_BYTE_OPENCL)


/* Color to use when textures are not found. */
#define TEX_IMAGE_MISSING_R 1
#define TEX_IMAGE_MISSING_G 0
#define TEX_IMAGE_MISSING_B 1
#define TEX_IMAGE_MISSING_A 1

CCL_NAMESPACE_END

#endif /* __UTIL_TEXTURE_H__ */
