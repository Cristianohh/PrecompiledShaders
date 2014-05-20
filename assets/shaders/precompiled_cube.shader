/* Copyright (c) 2014, Intel Corporation
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
*
* - Redistributions of source code must retain the above copyright notice, 
*   this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright notice, 
*   this list of conditions and the following disclaimer in the documentation 
*   and/or other materials provided with the distribution.
* - Neither the name of Intel Corporation nor the names of its contributors 
*   may be used to endorse or promote products derived from this software 
*   without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifdef FRAGMENT
precision mediump float;
#endif


varying vec4 v_Col;
varying vec3 v_normal;


uniform mat4 u_view_proj_matrix;
uniform mat4 u_world_matrix;

#ifdef VERTEX

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;

void main() 
{

    v_Col = a_color;

	// Transform normal to view space
    v_normal = ( u_world_matrix * vec4(a_normal, 0.0) ).xyz;

	// Convert position into screen space
	gl_Position = u_view_proj_matrix * u_world_matrix * vec4(a_position, 1.0);
	
}
#endif
 
 
#ifdef FRAGMENT

vec3 direction_to_light = vec3(0.0, 0.0, 1.0);
vec3 direction_to_eye = vec3(0.0, 0.0, 1.0);

void main()
{
	float diffuseFactor = dot(v_normal, direction_to_light);
	float specularFactor = pow(dot(reflect(direction_to_eye, v_normal), direction_to_light), 128.0);

	gl_FragColor = (diffuseFactor + specularFactor) * vec4(0.0 + v_Col.x, 0.4863 + v_Col.y, 0.5725 + v_Col.z, 1.0);
}

#endif