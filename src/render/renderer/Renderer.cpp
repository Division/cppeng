//
// Created by Sidorenko Nikita on 2/18/18.
//

#include "Renderer.h"
#import "render/mesh/Mesh.h"
#import "render/shader/Shader.h"
#import "Logging.h"
#import "glm/glm.hpp"

using namespace glm;

//const mat4 identityMatrix;
/*
void Renderer::renderMesh(const Mesh &mesh, Shader &shader, const mat4 *transform) {
  if (!mesh.hasVertices()) {
    ENGLog("Mesh data incomplete");
    return;
  }

  int indexCount = mesh.faceCount() * mesh.componentCount();
  int stride = mesh.strideBytes();
  if (!indexCount) {
    return; // Empty mesh
  }

//  let material = renderOpts && renderOpts.material;
//  let texture0 = material && material.texture0;
//  let texture1 = material && material.texture1;
//  let normalMap = material && material.normalMap;

  GLenum renderMode = GL_TRIANGLES;
//  if (renderOpts && renderOpts.renderMode !== undefined) {
//    renderMode = renderOpts.renderMode;
//  }

  if (!transform) {
    transform = &identityMatrix;
  }

  // Getting modelview matrix
  mat4.multiply(modelViewMatrix, this.viewMatrix, transform);

  // Getting normal matrix
  mat4.invert(normalMatrix, modelViewMatrix);
  mat4.transpose(normalMatrix, normalMatrix);

  // Shader uniforms
  shader.use();
  shader.setUniformMat4(UNIFORM_PROJECTION_MATRIX, this.projectionMatrix);
  shader.setUniformMat4(UNIFORM_MODELVIEW_MATRIX, modelViewMatrix);
  // TODO: add conditions for shader support of these features
  shader.setUniformMat4(UNIFORM_VIEW_MATRIX, this.viewMatrix);

  let currentTexture = 0;

  // Textures
  if (texture0) {
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture0);
    shader.setUniform1i(UNIFORM_TEXTURE0, 0);
    currentTexture += 1;
  }

  if (normalMap) {
    gl.activeTexture(gl.TEXTURE1);
    gl.bindTexture(gl.TEXTURE_2D, normalMap);
    shader.setUniform1i(UNIFORM_NORMAL_MAP, 1);
    currentTexture += 1;
  }

  // Vertex attribs

  gl.bindBuffer(gl.ARRAY_BUFFER, mesh.vbo);

  // Vertices
  if (mesh.hasVertices) {
    gl.enableVertexAttribArray(ATTRIBUTE_POSITION);
    gl.vertexAttribPointer(ATTRIBUTE_POSITION, 3, gl.FLOAT, false, stride, mesh.vertexOffsetBytes);
  }

  // TexCoord0
  if (mesh.hasTexCoord0) {
    gl.enableVertexAttribArray(ATTRIBUTE_TEXCOORD0);
    gl.vertexAttribPointer(ATTRIBUTE_TEXCOORD0, 2, gl.FLOAT, false, stride, mesh.texCoord0OffsetBytes);
  }

  if (mesh.hasNormals) {
    // Set normal matrix and light dir
    shader.setUniformMat4(UNIFORM_NORMAL_MATRIX, normalMatrix);
    shader.setUniform3(UNIFORM_LIGHT_DIR, this.lightDir);

    // Normals
    gl.enableVertexAttribArray(ATTRIBUTE_NORMAL);
    gl.vertexAttribPointer(ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, stride, mesh.normalOffsetBytes);
  }

  // TBN
  if (mesh.hasTBN) {
    // Tangents
    gl.enableVertexAttribArray(ATTRIBUTE_TANGENT);
    gl.vertexAttribPointer(ATTRIBUTE_TANGENT, 3, gl.FLOAT, false, stride, mesh.tangentOffsetBytes);
    // Bitangents
    gl.enableVertexAttribArray(ATTRIBUTE_BITANGENT);
    gl.vertexAttribPointer(ATTRIBUTE_BITANGENT, 3, gl.FLOAT, false, stride, mesh.bitangentOffsetBytes);
  }

  // Joint transforms
  if (renderOpts.jointTransforms) {
    let jointTransforms = this.setJointTransforms(renderOpts.jointTransforms);
    shader.setUniformMat4(UNIFORM_JOINT_TRANSFORMS, jointTransforms);
  }

  // Joint weights
  if (mesh.hasWeights) {
    gl.enableVertexAttribArray(ATTRIBUTE_JOINT_WEIGHTS);
    gl.vertexAttribPointer(ATTRIBUTE_JOINT_WEIGHTS, 3, gl.FLOAT, false, stride, mesh.weightOffsetBytes);

    gl.enableVertexAttribArray(ATTRIBUTE_JOINT_INDEXES);
    gl.vertexAttribPointer(ATTRIBUTE_JOINT_INDEXES, 3, gl.FLOAT, false, stride, mesh.jointIndexOffsetBytes);
  }

  // Colors
  if (mesh.hasColors) {
    gl.enableVertexAttribArray(ATTRIBUTE_COLOR);
    gl.vertexAttribPointer(ATTRIBUTE_COLOR, 4, gl.FLOAT, false, stride, mesh.colorOffsetBytes);
  }

  // Draw
  if (mesh.hasIndices) {
    // Indices
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
    gl.drawElements(renderMode, indexCount, gl.UNSIGNED_SHORT, 0);
  } else {
    gl.drawArrays(renderMode, 0, indexCount);
  }

}

 */


