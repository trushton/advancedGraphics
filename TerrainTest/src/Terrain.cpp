#include "Terrain.h"
#include "buffer.h"
#include "renderer.h"
#include "graphics.h"

Terrain::Terrain()
{
    model = glm::mat4(1);
}

Terrain::Terrain(string filename)
{
    fname = filename;
    model = glm::mat4(1);
}

void Terrain::setup()
{
    double x,y;
    getRawValuesFromFile(fname, vecs, min, max, xres, yres, projection,x,y,width,height);
    origin.x = x;
    origin.y = y;
    end.x = x + xres * (width);
    end.y = y - yres * (height);
    char* test = &projection[0];
    sr.importFromWkt(&test);
    createMesh(vecs, xres, yres, max, indices, vertexes);

    Renderer.init();
    Renderer.addShader(GL_VERTEX_SHADER,"../shaders/terrain.vert");
    Renderer.addShader(GL_FRAGMENT_SHADER,"../shaders/terrain.frag");
    cout << Renderer.compile() << endl;
    cout << Renderer.link() << endl;

    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    terrainpoints.generateBuffer(GL_ARRAY_BUFFER);
    terrainpoints.bindBuffer();
    // bind data buf.allocateBufferData(sizeof(Vertices),Vertices,GL_STATIC_DRAW);
    terrainpoints.allocateBufferData(sizeof(TerrainVertex)*vertexes.size(), &vertexes[0], GL_STATIC_DRAW);

    elements.generateBuffer(GL_ELEMENT_ARRAY_BUFFER);
    elements.bindBuffer();
    elements.allocateBufferData(sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

//    // Generate and populate the buffers with vertex attributes and the indices
//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes[0]) * vertexes.size(), &vertexes[0], GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vecs[0]) * vecs.size(), &vecs[0], GL_STATIC_DRAW);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Terrain::update(float dt)
{

}

void Terrain::render(glm::mat4& view, glm::mat4& projection)
{
    Renderer.useProgram();

    elements.bindBuffer();
    terrainpoints.bindBuffer();
    Renderer.enableVertexAttribPointer("poses");
    Renderer.enableVertexAttribPointer("TexCoord");
    Renderer.enableVertexAttribPointer("Normal");
    Renderer.setGLVertexAttribPointer("poses", 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, position));
    Renderer.setGLVertexAttribPointer("Normal", 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, normal));
    Renderer.setGLVertexAttribPointer("TexCoord", 2, GL_FLOAT, GL_TRUE, sizeof(TerrainVertex), (void*)offsetof(TerrainVertex, uv));


    glm::mat4 mvp = projection * view * model;
    Renderer.setUniformMatrix4x4("mvp", mvp);
    Renderer.setUniformMatrix4x4("model", model);
    Renderer.setUniformFloat("Max", max);
    Renderer.setUniformFloat("Min", min);

    Renderer.render(indices.size());
}


void Terrain::SetFile(string filename)
{
    fname = filename;
}

// The point must be in utm and in the same zone.
float Terrain::SampleTerrain(glm::vec2 point)
{

    // Calculate the normalized points
    auto normalized = (point-origin)/(end-origin);
    //normalized.y *= -1;
    cout << origin.y << " " << end.y << endl;
    cout << width << " " << height << endl;
    cout << "NORMALIZED X: " << normalized.x << " NROMALIZED Y: " << normalized.y << endl;
    if(normalized.x < 1 && normalized.x >= 0 && normalized.y < 1 && normalized.y >= 0)
    {
        int locx = (width-1) * normalized.x;
        int locy = (height-1) * normalized.y;
        //cout << locx << " " << locy << endl;
        return vecs[locx][locy];
    }
    return -1;
}