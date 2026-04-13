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
    mesh.calc_loop_triangles()    # <-- Triangles OK

    vertices_flat = []
    indices_flat = []
    uvs_flat = []
    normals_flat = []

    # Vertices
    for v in mesh.vertices:
        vertices_flat.extend([float(v.co.x), float(v.co.y), float(v.co.z)])

    # UVs
    has_uv = mesh.uv_layers.active is not None
    if has_uv:
        uv_layer = mesh.uv_layers.active.data

    # Loops normals DIRECTLY from mesh.loops
    for tri in mesh.loop_triangles:
        vert_order = [1,0,2]

        # Add triangle indices
        indices_flat.extend([
            tri.vertices[vert_order[0]],
            tri.vertices[vert_order[1]],
            tri.vertices[vert_order[2]],
        ])

        # For each loop in triangle
        for loop_id in [tri.loops[i] for i in vert_order]:

            # UV
            if has_uv:
                uv = uv_layer[loop_id].uv
                uvs_flat.extend([float(uv.x), float(uv.y)])

            # NORMAL from loop
            n = mesh.loops[loop_id].normal
            normals_flat.extend([float(n.x), float(n.y), float(n.z)])

    obj.to_mesh_clear()

    return {
        "vertices": vertices_flat,
        "indices": indices_flat,
        "uvs": uvs_flat,
        "normals": normals_flat
    }

def get_material_texture_name(obj):
    """Return only the image filename used by the object's active material."""
    mat = obj.active_material
    if not mat or not mat.node_tree:
        return None

    for node in mat.node_tree.nodes:
        if node.type == 'TEX_IMAGE' and node.image:
            full_path = bpy.path.abspath(node.image.filepath)
            return os.path.basename(full_path)   # <-- filename only

    return None

def export_scene(filepath):
    scene_data = {"objects": []}

    for obj in bpy.context.scene.objects:
        item = {}
        item["name"] = obj.name
        item["type"] = obj.type
        item["position"] = [float(x) for x in obj.matrix_world.to_translation()]
        rot = obj.rotation_euler.to_quaternion()
        item["rotation"] = [float(rot.x), float(rot.y), float(rot.z), float(rot.w)]
        item["scale"] = [float(x) for x in obj.matrix_world.to_scale()]
        item["parent"] = obj.parent.name if obj.parent else None
        item["texture"] = get_material_texture_name(obj)
        item["has_collider"] = obj["has_collider"]
        item["stock"] = obj["stock"]
        item["mesh"] = export_mesh(obj)
        scene_data["objects"].append(item)

    with open(filepath, "w") as f:
        json.dump(scene_data, f, indent=4)

    print("Exported:", filepath)

# Change the path to wherever you want to place the JSON
output_path = os.path.join("C:/Users/rboone/Documents/GitHub/Arena_Shooter/res/JSON", "json.json")
export_scene(output_path)