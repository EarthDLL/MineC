using Godot;
using Godot.Collections;
using Godot.NativeInterop;
using System;
using System.Collections;
using System.Runtime.CompilerServices;

public partial class Chunk : Node
{
    
    [Flags] enum WorldType { NORMAL  = 0 , OLD = 1 , FLAT = 2 , CUSTOM = 3}

    Vector3I chunk_size = new Vector3I(16,256,16);
    Vector3I start_point;
    Dictionary block_textures ;

    public int [ , , ] blocks = new int[16,256,16];

    Godot.Collections.Array arrays = new ();
    ArrayList vertices = new ArrayList();
    ArrayList uvs = new();


    public Chunk() : this(null,new Vector2I(0,0)){}
    public Chunk(Noise noise , Vector2I index){
        start_point = new Vector3I(index.X * chunk_size.X , 0 , index.Y * chunk_size.Z);
        GD.Print(start_point);
        arrays.Resize((int)Mesh.ArrayType.Max);
        for(int x = 0 ; x < chunk_size.X ; x++){
            for(int z = 0 ; z < chunk_size.Z ; z++){
                int high = (int)((Mathf.Clamp(noise.GetNoise2D(start_point.X + x , start_point.Z + z)+0.8,-1,1)+1)*32);
                for(int y = 0; y < high ;y++){
                    blocks[x,y,z] = 2;
                }
            }
        }

    }


    //0-下 1-上 2-X小 3-X大
    public void DrawFullBlockSide( Vector3 location , int id , int side){
        Vector2[] texture_pos = new Vector2[4];
        //Dictionary piece = (Dictionary)((GodotObject)block_infos[id]).Get("texture_pieces");
        location = (Vector3)start_point + location;
        switch(side)
        {
            case 0:
                location.X += 1;
                vertices.Add(location);
                location.X -= 1;
                vertices.Add(location);
                location.Z += 1;
                vertices.Add(location);
                vertices.Add(location);
                location.X += 1;
                vertices.Add(location);
                location.Z -= 1;
                vertices.Add(location);
                
                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["up"]);
                break;
            case 1:
                location.Y += 1;
                vertices.Add(location);
                location.X += 1;
                vertices.Add(location);
                location.Z += 1;
                vertices.Add(location);
                vertices.Add(location);
                location.X -= 1;
                vertices.Add(location);
                location.Z -= 1;
                vertices.Add(location);
                
                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["down"]);
                break;
            case 3:
                location += new Vector3(1,1,1);
                vertices.Add(location);
                location.Z -= 1;
                vertices.Add(location);
                location.Y -= 1;
                vertices.Add(location);
                vertices.Add(location);
                location.Z += 1;
                vertices.Add(location);
                location.Y += 1;
                vertices.Add(location);

                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["east"]);
                break;
            case 2:
                location.Y += 1;
                vertices.Add(location);
                location.Z += 1;
                vertices.Add(location);
                location.Y -= 1;
                vertices.Add(location);
                vertices.Add(location);
                location.Z -= 1;
                vertices.Add(location);
                location.Y += 1;
                vertices.Add(location);

                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["west"]);
                break;
            case 4:
                location.X += 1;
                location.Y += 1;
                vertices.Add(location);
                location.X -= 1;
                vertices.Add(location);
                location.Y -= 1;
                vertices.Add(location);
                vertices.Add(location);
                location.X += 1;
                vertices.Add(location);
                location.Y += 1;
                vertices.Add(location);

                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["north"]);
                break;
            case 5:
                location.Y += 1;
                location.Z += 1;
                vertices.Add(location);
                location.X += 1;
                vertices.Add(location);
                location.Y -= 1;
                vertices.Add(location);
                vertices.Add(location);
                location.X -= 1;
                vertices.Add(location);
                location.Y += 1;
                vertices.Add(location);

                uvs.AddRange((Vector2[])((Dictionary)block_textures[id])["south"]);
                break;
        }
    }


    public Mesh Render(Dictionary pieces){
        block_textures = pieces;
        ArrayMesh mesh = new();
        for(int x = 0 ; x < chunk_size.X ; x++){
            for(int z = 0 ; z < chunk_size.Z ; z++){
                for(int y = 0; y < chunk_size.Y ; y++){
                    int block_id = blocks[x,y,z];
                    if(block_id != 0){
                        if(y > 0){
                            if(blocks[x,y-1,z] == 0){
                                DrawFullBlockSide(new Vector3(x,y,z) , block_id , 0);
                            }
                        }else{
                            DrawFullBlockSide(new Vector3(x,y,z) , block_id , 0);
                        }
                        if(y < chunk_size.Y -1){
                            if(blocks[x,y+1,z] == 0){
                                DrawFullBlockSide( new Vector3(x,y,z) , block_id , 1);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) , block_id , 1);
                        }

                        if(x > 0){
                            if(blocks[x-1,y,z] == 0){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id , 2);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) , block_id , 2);
                        }
                        if(x < chunk_size.X -1){
                            if(blocks[x+1,y,z] == 0){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id, 3);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) ,block_id, 3);
                        }

                        if(z > 0){
                            if(blocks[x,y,z-1] == 0){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,4);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) ,block_id, 4);
                        }
                        if(z < chunk_size.Z -1){
                            if(blocks[x,y,z+1] == 0){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,5);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,5);
                        }
                    }
                }
            }
        }

        arrays[(int)Mesh.ArrayType.Vertex] = (Vector3[])vertices.ToArray(typeof(Vector3));
        arrays[(int)Mesh.ArrayType.TexUV] = (Vector2[])uvs.ToArray(typeof(Vector2));
        mesh.AddSurfaceFromArrays(Mesh.PrimitiveType.Triangles,arrays);
        vertices.Clear();
        uvs.Clear();

        return mesh;
    }
}
