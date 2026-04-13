import bpy

props = {
    "has_collider": True,
    "stock": False,
}

for obj in bpy.data.objects:
    for key, value in props.items():
        obj[key] = value