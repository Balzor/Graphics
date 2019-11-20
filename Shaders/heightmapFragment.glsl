# version 150 core

uniform sampler2D diffuseTex ;
uniform sampler2DShadow shadowTex ; // NEW !
uniform vec3 cameraPos ;
uniform sampler2D sandTex;

uniform vec4 lightColour[4] ;
uniform vec3 lightPos[4] ;
uniform float lightRadius[4] ;

in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 worldPos ;
 vec4 shadowProj;
} IN ;

out vec4 fragColour ;
float blend(float y){
 if(y<1050)
  return 0;
 if ( y >1150)
  return 1;
 float m = (y-1050)/100;
 return smoothstep(0.0,1.0,m);
}
//1265
void main ( void ) {
  vec4 diffuse = texture ( diffuseTex , IN . texCoord );
  vec4 sand= texture ( sandTex , IN . texCoord );

 vec4 texColour = mix(diffuse,sand,blend(IN.worldPos.y));
  for(int i =0; i < 4; i++){
   vec3 incident = normalize ( lightPos[i] - IN . worldPos );
   float lambert = max (0.0 , dot ( incident , IN . normal ));
   float dist = length ( lightPos[i] - IN . worldPos );
   float atten = 1.0 - clamp ( dist / lightRadius[i] , 0.0 , 1.0);
   vec3 viewDir = normalize ( cameraPos - IN . worldPos );
   vec3 halfDir = normalize ( incident + viewDir );


   float rFactor = max (0.0 , dot ( halfDir , IN . normal ));

   float sFactor = pow ( rFactor , 50.0 );

   float shadow = 1.0; // New !
   if( IN . shadowProj . w > 0.0) { // New !
    shadow = textureProj ( shadowTex , IN . shadowProj );
   }
   lambert *= shadow ; // New !

   vec3 colour = ( texColour . rgb * lightColour[i] . rgb );
   colour += ( lightColour[i] . rgb * sFactor ) * 0.33;

   fragColour += vec4 ( colour * atten * lambert , texColour . a );
   fragColour . rgb += ( texColour . rgb * lightColour[i] . rgb* atten * lambert ) * 0.1;


  if(IN.worldPos.y<20){
    //fragColour = vec4 ( 1,1,1,0);
    discard;
    //fragColour . rgb += ( diffuse . rgb * lightColour . rgb ) * 0.1;
   }/*else if(IN.worldPos.y<100){

    fragColour += vec4 ( 0.59,0.29,0.00,1)/2;
    fragColour . rgb += ( diffuse . rgb * lightColour[i] . rgb* atten * lambert ) * 0.1;
   }else{
    fragColour += vec4 ( colour * atten * lambert , diffuse . a );
    fragColour . rgb += ( diffuse . rgb * lightColour[i] . rgb ) * 0.1;
   }*/
  }



}