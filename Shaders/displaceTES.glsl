 # version 400

 layout ( triangles ) in ;

 uniform float time ;

 uniform mat4 modelMatrix ;
 uniform mat4 viewMatrix ;
 uniform mat4 projMatrix ;

  in Vertex { // Sent from the TCS
     vec2 texCoord ;
  } IN []; // Equal to TCS layout size

 out Vertex { // Each TES works on a single vertex !
     vec2 texCoord ;
 } OUT ;

// vec3 QuadMixVec3 ( vec3 a , vec3 b , vec3 c , vec3 d ) {
//     vec3 p0 = mix (a ,c , gl_TessCoord . x );
//     vec3 p1 = mix (b ,d , gl_TessCoord . x );
//
//     return mix ( p0 , p1 , gl_TessCoord . y );
// }
//
// vec2 QuadMixVec2 ( vec2 a , vec2 b , vec2 c , vec2 d ) {
//     vec2 p0 = mix (a ,c , gl_TessCoord . x );
//     vec2 p1 = mix (b ,d , gl_TessCoord . x );
//
//     return mix ( p0 , p1 , gl_TessCoord . y );
// }

 vec2 interpolate2D(vec2 v0,vec2 v1, vec2 v2){
     return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1+ vec2(gl_TessCoord.z )* v2;
 }
 vec3 interpolate3D(vec3 v0,vec3 v1, vec3 v2){
     return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1+ vec3(gl_TessCoord.z )* v2;
 }
  void main () {
      //vec3 combinedPos = QuadMixVec3 ( gl_in [0]. gl_Position . xyz ,
      vec3 combinedPos = interpolate3D ( gl_in [0]. gl_Position . xyz ,
      gl_in [1]. gl_Position . xyz ,
      gl_in [2]. gl_Position . xyz );
      //gl_in [3]. gl_Position . xyz );



      OUT . texCoord = interpolate2D ( IN [0]. texCoord ,
      IN [1]. texCoord ,
      IN [2]. texCoord );
      //IN [3]. texCoord );

      vec4 worldPos = modelMatrix * vec4 ( combinedPos , 1);
      float height = sin(time + worldPos.x) ;

      worldPos . y += height * 50;

      gl_Position = projMatrix * viewMatrix * worldPos ;
  }
