#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform samplerBuffer weightTex;
uniform samplerBuffer transformTex;

in  vec3 position;
in 	vec3 normal;
in 	vec3 tangent;
in  vec2 texCoord;

layout(location = 10) in vec2 weighting;

out Vertex	{
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
	vec4    shadowProj ; // New !

} OUT;

void main(void)	{
	int weightElements 	= int(weighting.x);	//These work!
	int weightIndex 	= int(weighting.y);	//These work!
	
	vec4 vertPos 	= vec4(0,0,0,1);
	vec3 oNormal  	= vec3(0,0,0);
	vec3 oTangent 	= vec3(0,0,0);
	
	for(int i = 0; i < weightElements; ++i) {	
		vec3 firstHalf  = texelFetch(weightTex,((weightIndex+i)*2)).rgb;
		vec3 weightPos  = texelFetch(weightTex,((weightIndex+i)*2)+1).rgb;
		
		mat4 jointTransform;
		jointTransform[0] = texelFetch(transformTex,(int(firstHalf.y)*8)+0);	//firstHalf.y = jointIndex
		jointTransform[1] = texelFetch(transformTex,(int(firstHalf.y)*8)+1);	//firstHalf.y = jointIndex
		jointTransform[2] = texelFetch(transformTex,(int(firstHalf.y)*8)+2);	//firstHalf.y = jointIndex
		jointTransform[3] = texelFetch(transformTex,(int(firstHalf.y)*8)+3);	//firstHalf.y = jointIndex
		
		mat4 invBindPose;
		invBindPose[0] = texelFetch(transformTex,(int(firstHalf.y)*8)+4);	//firstHalf.y = jointIndex
		invBindPose[1] = texelFetch(transformTex,(int(firstHalf.y)*8)+5);	//firstHalf.y = jointIndex
		invBindPose[2] = texelFetch(transformTex,(int(firstHalf.y)*8)+6);	//firstHalf.y = jointIndex
		invBindPose[3] = texelFetch(transformTex,(int(firstHalf.y)*8)+7);	//firstHalf.y = jointIndex	
			
		vertPos += (jointTransform * vec4(weightPos,1.0)) * firstHalf.z; 		// firstHalf.z = weightValue;
				
		oNormal  += (mat3(invBindPose) * mat3(jointTransform) * normal)  * firstHalf.z;
		oTangent += (mat3(invBindPose) * mat3(jointTransform) * tangent) * firstHalf.z;
	}
	mat3 normalMatrix = transpose(mat3(modelMatrix));
	
	vertPos.w = 1.0f;

	OUT.worldPos 	= (modelMatrix * vec4(vertPos.xyz, 1.0)).xyz;
	OUT.texCoord 	= texCoord;

	OUT.normal 		= normalMatrix * normalize(oNormal);
	OUT.tangent 	= normalMatrix * normalize(oTangent);

	OUT . shadowProj = ( shadowMatrix * vec4 ( position +( normal *1.5) ,1));

	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(vertPos.xyz, 1.0);
}