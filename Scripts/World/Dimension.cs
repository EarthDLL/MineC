using Godot;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Threading;

public partial class Dimension : Node3D
{
	String id = "minecraft:overworld";

	Node world ;

	Noise generater = GD.Load<Noise>("res://Resources/Noise/Noise.tres");
	StandardMaterial3D material = GD.Load<StandardMaterial3D>("res://Resources/Render/Blocks.tres");
    int count = 0;
	
	//const short max_view_range = 16;
	Vector3I chunk_size = new(16,64,16);
	Vector2I height_range = new(-64,320);
    Godot.Collections.Array players = new();

	Dictionary<Vector2I,short[ , , ]> chunk_blocks = new Dictionary<Vector2I, short[,,]>();
    Dictionary<Vector2I,short[ , ]> chunk_render_list = new();
	Dictionary<Vector2I,ChunkInstance> chunkers = new();

	Godot.Collections.Array<bool> full_block_list;
	Godot.Collections.Array textures ;
    Godot.Collections.Array block_vertices ;
	ArrayList vertices = new ArrayList();
    ArrayList uvs = new();
    ArrayList uv2s = new ArrayList();
	Vector3 start_point;

	GodotThread thread = new();

	public Dimension() : this(null,null){}
    public Dimension(Node World , Node BlockManager){
		world = World;
		full_block_list = (Godot.Collections.Array<bool>)BlockManager.Get("full_block_list");
		textures = (Godot.Collections.Array)BlockManager.Get("block_texture_pieces");
        block_vertices = (Godot.Collections.Array)BlockManager.Get("block_vertices");
	}

    public override void _Ready()
	{
		//GD.Print(Time.GetTicksMsec());
		for(short i=0;i<18;i++){
			for(short i2=0;i2<18;i2++){
				generate_chunk(new Vector2I(i,i2));
			}
		}
        
        GD.Print(Time.GetTicksMsec());
		for(short i=1;i<16;i++){
			for(short i2=1;i2<16;i2++){
				render_chunk(new Vector2I(i,i2));
			}
		}
        generate_chunk(new Vector2I(0,0));
	}

    public override void _PhysicsProcess(double delta)
    {
        if(thread.IsStarted()){
            if(!thread.IsAlive()){
                thread.WaitToFinish();
                GD.Print(count);
                thread = new GodotThread();
            }
        }
        if(!thread.IsStarted() && rendering_pool.Count > 0){
            Vector2I vec = (Vector2I)rendering_pool[0];
            thread.Start(Callable.From(()=>{
                force_render_chunk(vec);
            }));
            rendering_pool.RemoveAt(0);
        }

        if(Engine.GetPhysicsFrames() % 60 == 0){
            foreach(GodotObject player in players){
                Vector2 pos = (Vector2)player.Get("position");
                Vector2I index = get_chunk_index(pos);
                load_nearby_chunks(index,(int)player.Get("view_range"));
            }
        }
    }

    public void load_nearby_chunks(Vector2I index , int range){
        
    }

    public Vector2I get_chunk_index(Vector2 pos){
        return new Vector2I((int)Math.Floor(pos.X/chunk_size.X),(int)Math.Floor(pos.Y/chunk_size.Y));
    }

    public bool has_chunk (Vector2I index){
		return chunk_blocks.ContainsKey(index);
	}

    //0-下 1-上 2-X小 3-X大
	public void DrawFullBlockSide( Vector3 location , int id , int side){
        location = start_point + location;
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
                
                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[0]);
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
                
                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[1]);
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

                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[2]);
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

                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[3]);
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

                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[4]);
                break;
            case 5:
                vertices.AddRange((Vector3[])((Godot.Collections.Dictionary)block_vertices[id])[5]);
                uvs.AddRange((Vector2[])((Godot.Collections.Dictionary)textures[id])[5]);
				
                break;
        }
    }


    public bool is_block_full(short block_id){
		if(block_id == 0){
			return false;
		}
		return true;
	}

    ArrayList rendering_pool = new();
    public void render_chunk(Vector2I index){
        rendering_pool.Add(index);
    }


	public void force_render_chunk(Vector2I index){
        double time = Time.GetUnixTimeFromSystem();
        GD.Print("Start to Render the Chunk!");
		Godot.Collections.Array arrays = new ();
		arrays.Resize((int)Mesh.ArrayType.Max);
		if(has_chunk(new Vector2I(index.X+1,index.Y))&& has_chunk(new Vector2I(index.X,index.Y+1)) && has_chunk(new Vector2I(index.X-1,index.Y)) && has_chunk(new Vector2I(index.X,index.Y-1))){
			start_point = new Vector3(index.X * chunk_size.X , 0 , index.Y * chunk_size.Z);
			ChunkInstance instance ;
			if(chunkers.ContainsKey(index)){
				instance = chunkers[index];
			}else{
				instance = new ChunkInstance();
				CallDeferred(MethodName.AddChild,instance);
				chunkers[index] = instance;
			}
			
			ArrayMesh mesh = new();
			float high = height_range.Y-height_range.X;
			short[ , , ] blocks = chunk_blocks[index];
            GD.Print("Preparing Works.Used Time:",Time.GetUnixTimeFromSystem() - time);
            time = Time.GetUnixTimeFromSystem();

			for(int x = 0 ; x < chunk_size.X ; x++){
            for(int z = 0 ; z < chunk_size.Z ; z++){
                for(int y = 0; y <  high ; y++){
                    int block_id = blocks[x,y,z];
                    if(block_id != 0){
                        if(y > 0){
                            if(!full_block_list[blocks[x,y-1,z]]){
                                DrawFullBlockSide(new Vector3(x,y,z) , block_id , 0);
                            }
                        }else{
                            DrawFullBlockSide(new Vector3(x,y,z) , block_id , 0);
                        }
                        if(y < high -1){
                            if(!full_block_list[blocks[x,y+1,z]]){
                                DrawFullBlockSide( new Vector3(x,y,z) , block_id , 1);
                            }
                        }else{
                            DrawFullBlockSide( new Vector3(x,y,z) , block_id , 1);
                        }

                        if(x > 0){
                            if(!full_block_list[blocks[x-1,y,z]]){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id , 2);
                            }
                        }else{
							if(!full_block_list[chunk_blocks[new Vector2I(index.X-1,index.Y)][chunk_size.X -1 , y ,z]]){
                            	DrawFullBlockSide( new Vector3(x,y,z) , block_id , 2);
							}
                        }
                        if(x < chunk_size.X -1){
                            if(!full_block_list[blocks[x+1,y,z]]){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id, 3);
                            }
                        }else{
							if(!full_block_list[chunk_blocks[new Vector2I(index.X+1,index.Y)][0 , y ,z]]){
                            	DrawFullBlockSide( new Vector3(x,y,z) ,block_id, 3);
							}
                        }

                        if(z > 0){
                            if(!full_block_list[blocks[x,y,z-1]]){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,4);
                            }
                        }else{
							if(!full_block_list[chunk_blocks[new Vector2I(index.X,index.Y-1)][x , y ,chunk_size.Z-1]]){
                           		DrawFullBlockSide(new Vector3(x,y,z) ,block_id, 4);
							}
                        }
                        if(z < chunk_size.Z -1){
                            if(!full_block_list[blocks[x,y,z+1]]){
                                DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,5);
                            }
                        }else{
							if(!full_block_list[chunk_blocks[new Vector2I(index.X,index.Y+1)][x , y ,0]]){
                            	DrawFullBlockSide( new Vector3(x,y,z) ,block_id ,5);
							}
                        }
                    }
                }
            }
        }
        GD.Print("Finishing Rendering.Used Time:",Time.GetUnixTimeFromSystem() - time);
        time = Time.GetUnixTimeFromSystem();
		arrays[(int)Mesh.ArrayType.Vertex] = (Vector3[])vertices.ToArray(typeof(Vector3));
        arrays[(int)Mesh.ArrayType.TexUV] = (Vector2[])uvs.ToArray(typeof(Vector2));
        mesh.AddSurfaceFromArrays(Mesh.PrimitiveType.Triangles,arrays);
		instance.CallDeferred(MethodName.Set,"mesh",mesh);
		instance.MaterialOverride = material;
		uvs.Clear();
		vertices.Clear();
        GD.Print("Finished.Used Time:",Time.GetUnixTimeFromSystem() - time);
        time = Time.GetUnixTimeFromSystem();
		}
	}

	

	public void generate_chunk(Vector2I index){
		Vector3I start_point = new Vector3I(index.X * chunk_size.X , 0 , index.Y * chunk_size.Z);

		short[,,] blocks = new short[chunk_size.X,height_range.Y-height_range.X,chunk_size.Z];
        chunk_render_list[index] = new short[chunk_size.Y,chunk_size.Z];
        for(short x = 0 ; x < chunk_size.X ; x++){
            for(short z = 0 ; z < chunk_size.Z ; z++){
                short high = (short)((Mathf.Clamp(generater.GetNoise2D(start_point.X + x , start_point.Z + z)+0.5,-1,1)+1)*32 + 64);
                for(short y = 0; y < high ;y++){
                    set_block_in_chunk(blocks,x,y,z,2);
                }
            }
        }

		chunk_blocks[index] = blocks;
	}

    public void set_block_in_chunk(short[,,] chunk , int x , int y , int z , short id){
        chunk[x,y,z] = id;
    }

    public void fill_blocks_in_chunk(Vector2I index, Vector3I from , Vector3I to , short id){
        short[,,] chunk = chunk_blocks[index];
        int y_side = (to.Y - from.Y)/Math.Abs(to.Y - from.Y);
        int z_side = (to.Z - from.Z)/Math.Abs(to.Z - from.Z);
        int x_side = (to.X - from.X)/Math.Abs(to.X - from.X);
        for(int y = from.Y ; y != to.Y + y_side ; y += y_side){
            for(int z = from.Z ; z != to.Z + z_side; z += z_side){
                for(int x = from.X ; x != to.X + x_side; x += x_side){
                    chunk[x,y,z] = id;
                }
            }
        }
        if(full_block_list[id]){
            Vector3I size = to - from;
            if(Math.Abs(size.X) > 2 && Math.Abs(size.Y) > 2 && Math.Abs(size.Z) > 2){
                for(int y = from.Y + y_side ; y != to.Y ; y += y_side){
                    for(int z = from.Z + z_side; z != to.Z ; z += z_side){
                        for(int x = from.X + x_side; x != to.X ; x += x_side){
                            chunk_render_list[] = 
                        }
                    }
                }
            }
        }
        
    }
}