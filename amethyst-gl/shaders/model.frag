#version 330

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D baseTex;

layout(location = 0) out vec4 FragColor;

struct LightInfo {
    vec4 Position;
    vec3 Intensity;
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

void phongModel( vec3 pos, vec3 norm, out vec3 amdAndDiff, out vec3 spec)
{
    //vec3 n = normalize( norm );
    vec3 s = normalize( vec3(Light.Position) - pos );
    vec3 v = normalize( vec3(-pos));
    vec3 h = normalize( v + s );
    //vec3 r = reflect( -s, n );
    vec3 ambient = Light.Intensity * Material.Ka;
    vec3 diffuse = Light.Intensity * Material.Kd * max(0.0, dot(s, Normal) );
    spec = Light.Intensity * Material.Ks * pow( max(0.0, dot(h, Normal) ), Material.Shininess );
    amdAndDiff = ambient + diffuse;

}

void main()
{
    vec3 amdAndDiff, spec;
    vec4 texColor = texture(baseTex, TexCoord.st);
    phongModel(Position, Normal, amdAndDiff, spec);
    FragColor = vec4(amdAndDiff, 1.0) * texColor + vec4(spec, 1.0);
}