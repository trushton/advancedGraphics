#version 410

layout(points) in;
layout(points) out;
layout(max_vertices = 93) out;

in float Type0[];
in vec3 Position0[];
in vec3 Velocity0[];
in float Age0[];
in vec3 Color0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;
out vec3 Color1;

uniform float dt;
uniform float time;
uniform sampler1D random_texture;
uniform float launcher_lifetime;
uniform float shell_lifetime;
uniform float secondary_shell_lifetime;
uniform vec3 particleColor;

float PARTICLE_TYPE_LAUNCHER =  0.0f;
float PARTICLE_TYPE_SHELL = 1.0f;
float PARTICLE_TYPE_SECONDARY_SHELL = 2.0f;

vec3 GetRandomDir(float TexCoord)
{
     vec3 Dir = texture(random_texture, TexCoord).xyz;
     Dir -= vec3(0.5, 0.5, 0.5);
     return Dir;
}

void main()
{
    float Age = Age0[0] + dt;

                Color1 = Color0[0];


    if (Type0[0] == PARTICLE_TYPE_LAUNCHER) {
        if (Age >= launcher_lifetime) {
            Type1 = PARTICLE_TYPE_SHELL;
            Position1 = Position0[0];
            vec3 Dir = GetRandomDir(time/1000.0);
            Dir.y = max(Dir.y, 0.5);
            Velocity1 = normalize(Dir) * 150.0;
            Age1 = texture(random_texture, time/1000.0).x;
            EmitVertex();
            EndPrimitive();
            Age = 0.0;
                Color1 = particleColor;

        }

        Type1 = PARTICLE_TYPE_LAUNCHER;
        Position1 = Position0[0];
        Velocity1 = Velocity0[0];
        Age1 = Age;
        EmitVertex();
        EndPrimitive();
    }
    else {
        float DeltaTimeSecs = dt / 1000.0f;
        float t1 = Age0[0] / 1000.0;
        float t2 = Age / 1000.0;
        vec3 DeltaP = DeltaTimeSecs * Velocity0[0];
        vec3 DeltaV = vec3(DeltaTimeSecs) * (0.0, -0.981, 0.0);


        if (Type0[0] == PARTICLE_TYPE_SHELL)  {
            if (Age < shell_lifetime) {
                Type1 = PARTICLE_TYPE_SHELL;
                Position1 = Position0[0] + DeltaP;
                Velocity1 = Velocity0[0] + DeltaV;
                Age1 = Age;
                EmitVertex();
                EndPrimitive();
            }
            else {
                for (int i = 0 ; i < 100 ; i++) {
                     Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                     Position1 = Position0[0];
                     vec3 Dir = GetRandomDir((time + i)/1000.0);
                     Velocity1 = normalize(Dir) * 150;
                     Age1 = 0.0f;
                     EmitVertex();
                     EndPrimitive();
                }
            }
        }
        else {
            if (Age < secondary_shell_lifetime) {
                Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
                Position1 = Position0[0] + DeltaP;
                Velocity1 = Velocity0[0] + DeltaV;
                Age1 = Age;
                EmitVertex();
                EndPrimitive();
            }
        }
    }
}
