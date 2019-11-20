 # version 150 core

 uniform sampler2D diffuseTex ;
 uniform samplerCube cubeTex ;

 uniform vec4 lightColour[4] ;
 uniform vec3 lightPos[4] ;
 uniform float lightRadius[4] ;

 uniform vec3 cameraPos ;

 in Vertex {
 vec4 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 worldPos ;
 } IN ;

 out vec4 fragColour ;
 void main ( void ) {
  for(int i =0; i< 4; i++){
   vec4 diffuse = texture ( diffuseTex , IN . texCoord ) * IN . colour ;
   vec3 incident = normalize ( IN . worldPos - cameraPos );
   float dist = length ( lightPos[i] - IN . worldPos );
   float atten = 1.0 - clamp ( dist / lightRadius[i] , 0.2 , 1.0);
   vec4 reflection = texture ( cubeTex , reflect ( incident , normalize ( IN . normal )));

   fragColour += ( lightColour[i] * diffuse * atten )*( diffuse + reflection );
  }
 }