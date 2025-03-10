 # version 150 core

 uniform sampler2D diffuseTex ;
 uniform sampler2D bumpTex ;
 uniform sampler2DShadow shadowTex ; // NEW !

 uniform vec3 cameraPos ;

 uniform vec4 lightColour[4] ;
 uniform vec3 lightPos[4] ;
 uniform float lightRadius[4] ;

 in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 tangent ;
 vec3 binormal ;
 vec3 worldPos ;
 vec4 shadowProj ; // New !
 } IN ;

 out vec4 fragColour ;

 void main ( void ) {
 mat3 TBN = mat3 ( IN . tangent , IN . binormal , IN . normal );
 vec3 normal = normalize ( TBN *
 ( texture2D ( bumpTex , IN . texCoord ). rgb * 2.0 - 1.0));


  //vec2 test = IN.texCoord;
  //test.y = 1.0f - test.y;
 vec4 diffuse = texture2D ( diffuseTex , IN . texCoord );


  for(int i =0; i<4;i++){
   vec3 incident = normalize ( lightPos[i] - IN . worldPos );
   float lambert = max (0.0 , dot ( incident , normal )); // Different !
   float dist = length ( lightPos[i] - IN . worldPos );
   float atten = 1.0 - clamp ( dist / lightRadius[i] , 0.0 , 1.0);
   vec3 viewDir = normalize ( cameraPos - IN . worldPos );
   vec3 halfDir = normalize ( incident + viewDir );

   float rFactor = max (0.0 , dot ( halfDir , normal )); // Different !
   float sFactor = pow ( rFactor , 33.0 );

   float shadow = 1.0; // New !
   if( IN . shadowProj . w > 0.0) { // New !
    shadow = textureProj ( shadowTex , IN . shadowProj );
   }
   lambert *= shadow ; // New !

   vec3 colour = ( diffuse . rgb * lightColour[i] . rgb );
   colour += ( lightColour[i] . rgb * sFactor ) * 0.33;
   fragColour += vec4 ( colour * atten * lambert , diffuse . a );
   fragColour . rgb += ( diffuse . rgb * lightColour[i] . rgb ) * 0.1;
  }
  //FragColor = texture(diffuseTex, IN.texCoord);

 //fragColour . rgb =diffuse.rgb;
 }