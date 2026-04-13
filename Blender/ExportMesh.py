import bpy
import json
import mathutils
import os

def vec(v):
    return [float(v[0]), float(v[1]), float(v[2])]

def export_mesh(obj, apply_world_matrix=False):
    if obj.type != 'MESH':
        return None

    mesh = obj.to_mesh()
    mesh.calc_loop_triangles()
    mesh.calc_normals_split()  # calcule les normales par loop

    vertices_flat = []
    indices_flat = []
    uvs_flat = []
    normals_flat = []

    for v in mesh.vertices:
        pos = v.co
        vertices_flat.extend([float(pos[0]), float(pos[1]), float(pos[2])])
        # normale du vertex (local space)
        normals_flat.extend([float(v.normal.x), float(v.normal.y), float(v.normal.z)])

    for tri in mesh.loop_triangles:
        indices_flat.extend([int(tri.vertices[0]), int(tri.vertices[1]), int(tri.vertices[2])])

    # Export UVs
    if len(mesh.uv_layers) > 0:
        uv_per_vertex = [[0.0, 0.0] for _ in range(len(mesh.vertices))]
        uv_layer = mesh.uv_layers.active.data
        for loop in mesh.loops:
            vidx = loop.vertex_index
            uv = uv_layer[loop.index].uv
            uv_per_vertex[vidx] = [float(uv.x), float(uv.y)]
        for uv in uv_per_vertex:
            uvs_flat.extend([uv[0], uv[1]])
    else:
        uvs_flat = []

    obj.to_mesh_clear()

    return {
        "vertices": vertices_flat,
        "indices": indices_flat,
        "uvs": uvs_flat,
        "normals": normals_flat
    }

def export_scene(filepath):
    scene_data = {"objects": []}

    for obj in bpy.context.scene.objects:
        item = {}
        item["name"] = obj.name
        item["type"] = obj.type
        item["position"] = [float(x) for x in obj.matrix_world.to_translation()]
        rot = obj.matrix_world.to_quaternion()
        item["rotation"] = [float(rot.x), float(rot.y), float(rot.z), float(rot.w)]
        item["scale"] = [float(x) for x in obj.matrix_world.to_scale()]
        item["parent"] = obj.parent.name if obj.parent else None
        item["material"] = obj.active_material.name if obj.active_material else None
        item["mesh"] = export_mesh(obj)
        scene_data["objects"].append(item)

    with open(filepath, "w") as f:
        json.dump(scene_data, f, indent=4)

    print("Exported:", filepath)

# Change the path to wherever you want to place the JSON
output_path = os.path.join("C:/BlenderExportJson", "blender_scene_v3.json")
export_scene(output_path)
