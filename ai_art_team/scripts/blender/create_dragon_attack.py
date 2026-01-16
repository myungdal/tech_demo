"""
Blender Python Script: Create Dragon Character with Attack Animation
Usage: blender --background --python create_dragon_attack.py
"""

import bpy
import math
import sys

def clear_scene():
    """Clear all objects from the scene"""
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete(use_global=False)

def create_material(name, color):
    """Create a simple material with given color (R, G, B, A)"""
    mat = bpy.data.materials.new(name=name)
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes["Principled BSDF"]
    bsdf.inputs['Base Color'].default_value = color
    return mat

def create_cube(name, location, scale, rotation=(0,0,0), material=None):
    """Create a cube with given parameters"""
    bpy.ops.mesh.primitive_cube_add(size=1, location=location, rotation=rotation)
    obj = bpy.context.active_object
    obj.name = name
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
    
    if material:
        obj.data.materials.append(material)
    
    return obj

def create_cone(name, location, scale, rotation=(0,0,0), material=None):
    """Create a cone"""
    bpy.ops.mesh.primitive_cone_add(radius1=0.5, radius2=0, depth=1, location=location, rotation=rotation)
    obj = bpy.context.active_object
    obj.name = name
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
    
    if material:
        obj.data.materials.append(material)
    
    return obj

def create_dragon_mesh():
    """Create dragon character from primitive shapes"""
    # Materials
    body_mat = create_material("DragonBody", (0.2, 0.5, 0.2, 1.0))  # Green scales
    belly_mat = create_material("DragonBelly", (0.6, 0.7, 0.3, 1.0))  # Lighter belly
    wing_mat = create_material("DragonWing", (0.3, 0.6, 0.3, 1.0))  # Wing membrane
    horn_mat = create_material("DragonHorn", (0.3, 0.25, 0.2, 1.0))  # Dark horns
    eye_mat = create_material("DragonEye", (1.0, 0.8, 0.0, 1.0))  # Yellow eyes
    
    parts = []
    
    # === BODY ===
    # Main body (large horizontal)
    body = create_cube("Body", (0, 0, 0.8), (1.2, 0.6, 0.5), material=body_mat)
    parts.append(body)
    
    # Belly (slightly lighter, underneath)
    belly = create_cube("Belly", (0, 0, 0.65), (1.0, 0.5, 0.2), material=belly_mat)
    parts.append(belly)
    
    # === HEAD ===
    # Head base
    head = create_cube("Head", (0.9, 0, 1.0), (0.4, 0.35, 0.35), material=body_mat)
    parts.append(head)
    
    # Snout (elongated)
    snout = create_cube("Snout", (1.3, 0, 0.95), (0.4, 0.2, 0.2), material=body_mat)
    parts.append(snout)
    
    # Lower jaw
    jaw = create_cube("Jaw", (1.2, 0, 0.85), (0.35, 0.18, 0.1), material=body_mat)
    parts.append(jaw)
    
    # Eyes
    eye_l = create_cube("Eye_L", (0.95, 0.15, 1.1), (0.08, 0.08, 0.08), material=eye_mat)
    parts.append(eye_l)
    eye_r = create_cube("Eye_R", (0.95, -0.15, 1.1), (0.08, 0.08, 0.08), material=eye_mat)
    parts.append(eye_r)
    
    # Horns
    horn_l = create_cone("Horn_L", (0.7, 0.15, 1.25), (0.08, 0.08, 0.25), 
                         rotation=(0.3, 0, 0.2), material=horn_mat)
    parts.append(horn_l)
    horn_r = create_cone("Horn_R", (0.7, -0.15, 1.25), (0.08, 0.08, 0.25), 
                         rotation=(-0.3, 0, -0.2), material=horn_mat)
    parts.append(horn_r)
    
    # === NECK ===
    neck = create_cube("Neck", (0.55, 0, 0.95), (0.3, 0.25, 0.3), material=body_mat)
    parts.append(neck)
    
    # === WINGS ===
    # Wing bones (upper arm)
    wing_bone_l = create_cube("WingBone_L", (0, 0.5, 1.1), (0.1, 0.6, 0.08), 
                              rotation=(0, 0, 0.5), material=body_mat)
    parts.append(wing_bone_l)
    wing_bone_r = create_cube("WingBone_R", (0, -0.5, 1.1), (0.1, 0.6, 0.08), 
                              rotation=(0, 0, -0.5), material=body_mat)
    parts.append(wing_bone_r)
    
    # Wing membrane (flat triangular shape approximated with cube)
    wing_mem_l = create_cube("WingMem_L", (-0.2, 0.7, 1.0), (0.5, 0.4, 0.02), 
                             rotation=(0, 0, 0.3), material=wing_mat)
    parts.append(wing_mem_l)
    wing_mem_r = create_cube("WingMem_R", (-0.2, -0.7, 1.0), (0.5, 0.4, 0.02), 
                             rotation=(0, 0, -0.3), material=wing_mat)
    parts.append(wing_mem_r)
    
    # === LEGS ===
    # Front legs
    leg_fl = create_cube("Leg_FL", (0.5, 0.3, 0.4), (0.12, 0.12, 0.5), material=body_mat)
    parts.append(leg_fl)
    leg_fr = create_cube("Leg_FR", (0.5, -0.3, 0.4), (0.12, 0.12, 0.5), material=body_mat)
    parts.append(leg_fr)
    
    # Back legs (bigger)
    leg_bl = create_cube("Leg_BL", (-0.4, 0.35, 0.35), (0.15, 0.15, 0.55), material=body_mat)
    parts.append(leg_bl)
    leg_br = create_cube("Leg_BR", (-0.4, -0.35, 0.35), (0.15, 0.15, 0.55), material=body_mat)
    parts.append(leg_br)
    
    # Feet/claws
    foot_fl = create_cube("Foot_FL", (0.55, 0.3, 0.1), (0.15, 0.15, 0.08), material=horn_mat)
    parts.append(foot_fl)
    foot_fr = create_cube("Foot_FR", (0.55, -0.3, 0.1), (0.15, 0.15, 0.08), material=horn_mat)
    parts.append(foot_fr)
    foot_bl = create_cube("Foot_BL", (-0.35, 0.35, 0.08), (0.18, 0.18, 0.08), material=horn_mat)
    parts.append(foot_bl)
    foot_br = create_cube("Foot_BR", (-0.35, -0.35, 0.08), (0.18, 0.18, 0.08), material=horn_mat)
    parts.append(foot_br)
    
    # === TAIL ===
    tail1 = create_cube("Tail1", (-0.8, 0, 0.75), (0.35, 0.2, 0.2), material=body_mat)
    parts.append(tail1)
    tail2 = create_cube("Tail2", (-1.1, 0, 0.7), (0.3, 0.15, 0.15), material=body_mat)
    parts.append(tail2)
    tail3 = create_cube("Tail3", (-1.35, 0, 0.65), (0.25, 0.1, 0.1), material=body_mat)
    parts.append(tail3)
    # Tail spike
    tail_spike = create_cone("TailSpike", (-1.55, 0, 0.6), (0.1, 0.1, 0.2), 
                             rotation=(0, -1.57, 0), material=horn_mat)
    parts.append(tail_spike)
    
    return parts

def create_dragon_armature():
    """Create armature (skeleton) for the dragon"""
    bpy.ops.object.armature_add(enter_editmode=True, location=(0, 0, 0))
    armature = bpy.context.active_object
    armature.name = "DragonArmature"
    
    arm_data = armature.data
    arm_data.name = "DragonRig"
    
    # Clear default bone
    bpy.ops.armature.select_all(action='SELECT')
    bpy.ops.armature.delete()
    
    # Dragon bone structure
    bones_info = {
        'root': {'head': (0, 0, 0.8), 'tail': (0, 0, 0.5), 'parent': None},
        'spine': {'head': (0, 0, 0.8), 'tail': (0.4, 0, 0.85), 'parent': 'root'},
        'neck': {'head': (0.4, 0, 0.85), 'tail': (0.7, 0, 1.0), 'parent': 'spine'},
        'head': {'head': (0.7, 0, 1.0), 'tail': (1.1, 0, 0.95), 'parent': 'neck'},
        'jaw': {'head': (0.9, 0, 0.9), 'tail': (1.2, 0, 0.85), 'parent': 'head'},
        
        'wing_L': {'head': (0, 0.3, 1.0), 'tail': (0, 0.8, 1.2), 'parent': 'spine'},
        'wing_R': {'head': (0, -0.3, 1.0), 'tail': (0, -0.8, 1.2), 'parent': 'spine'},
        
        'leg_FL': {'head': (0.5, 0.3, 0.65), 'tail': (0.5, 0.3, 0.1), 'parent': 'spine'},
        'leg_FR': {'head': (0.5, -0.3, 0.65), 'tail': (0.5, -0.3, 0.1), 'parent': 'spine'},
        'leg_BL': {'head': (-0.4, 0.35, 0.6), 'tail': (-0.4, 0.35, 0.08), 'parent': 'root'},
        'leg_BR': {'head': (-0.4, -0.35, 0.6), 'tail': (-0.4, -0.35, 0.08), 'parent': 'root'},
        
        'tail1': {'head': (-0.6, 0, 0.75), 'tail': (-0.95, 0, 0.7), 'parent': 'root'},
        'tail2': {'head': (-0.95, 0, 0.7), 'tail': (-1.25, 0, 0.65), 'parent': 'tail1'},
        'tail3': {'head': (-1.25, 0, 0.65), 'tail': (-1.55, 0, 0.6), 'parent': 'tail2'},
    }
    
    created_bones = {}
    
    for bone_name, info in bones_info.items():
        bone = arm_data.edit_bones.new(bone_name)
        bone.head = info['head']
        bone.tail = info['tail']
        created_bones[bone_name] = bone
    
    # Set parent relationships
    for bone_name, info in bones_info.items():
        if info['parent']:
            created_bones[bone_name].parent = created_bones[info['parent']]
    
    bpy.ops.object.mode_set(mode='OBJECT')
    
    return armature

def parent_mesh_to_armature(mesh_parts, armature):
    """Parent mesh parts to armature with automatic weights"""
    bpy.ops.object.select_all(action='DESELECT')
    
    for part in mesh_parts:
        part.select_set(True)
    
    bpy.context.view_layer.objects.active = mesh_parts[0]
    bpy.ops.object.join()
    
    merged_mesh = bpy.context.active_object
    merged_mesh.name = "Dragon"
    
    bpy.ops.object.select_all(action='DESELECT')
    merged_mesh.select_set(True)
    armature.select_set(True)
    bpy.context.view_layer.objects.active = armature
    
    bpy.ops.object.parent_set(type='ARMATURE_AUTO')
    
    return merged_mesh

def create_attack_animation(armature):
    """Create dragon attack animation (bite attack)"""
    bpy.context.scene.frame_start = 1
    bpy.context.scene.frame_end = 70  # 7 frames at 10fps = 0.7 seconds
    bpy.context.scene.render.fps = 10
    
    action = bpy.data.actions.new(name="Attack")
    armature.animation_data_create()
    armature.animation_data.action = action
    
    bpy.context.view_layer.objects.active = armature
    bpy.ops.object.mode_set(mode='POSE')
    
    # Get bones
    head = armature.pose.bones.get('head')
    neck = armature.pose.bones.get('neck')
    jaw = armature.pose.bones.get('jaw')
    wing_l = armature.pose.bones.get('wing_L')
    wing_r = armature.pose.bones.get('wing_R')
    tail1 = armature.pose.bones.get('tail1')
    tail2 = armature.pose.bones.get('tail2')
    tail3 = armature.pose.bones.get('tail3')
    
    # === Frame 1: Ready stance ===
    bpy.context.scene.frame_set(1)
    
    if head:
        head.rotation_euler = (0, 0, 0)
        head.keyframe_insert(data_path="rotation_euler", frame=1)
    if neck:
        neck.rotation_euler = (0, 0, 0)
        neck.keyframe_insert(data_path="rotation_euler", frame=1)
    if jaw:
        jaw.rotation_euler = (0, 0, 0)
        jaw.keyframe_insert(data_path="rotation_euler", frame=1)
    if wing_l:
        wing_l.rotation_euler = (0, 0, 0)
        wing_l.keyframe_insert(data_path="rotation_euler", frame=1)
    if wing_r:
        wing_r.rotation_euler = (0, 0, 0)
        wing_r.keyframe_insert(data_path="rotation_euler", frame=1)
    if tail1:
        tail1.rotation_euler = (0, 0, 0)
        tail1.keyframe_insert(data_path="rotation_euler", frame=1)
    
    # === Frame 20: Wind up (pull head back, open jaw, raise wings) ===
    bpy.context.scene.frame_set(20)
    
    if head:
        head.rotation_euler = (0.3, 0, 0)  # Tilt up
        head.keyframe_insert(data_path="rotation_euler", frame=20)
    if neck:
        neck.rotation_euler = (-0.2, 0, 0)  # Pull back
        neck.keyframe_insert(data_path="rotation_euler", frame=20)
    if jaw:
        jaw.rotation_euler = (0.4, 0, 0)  # Open mouth
        jaw.keyframe_insert(data_path="rotation_euler", frame=20)
    if wing_l:
        wing_l.rotation_euler = (0, 0, 0.5)  # Raise wing
        wing_l.keyframe_insert(data_path="rotation_euler", frame=20)
    if wing_r:
        wing_r.rotation_euler = (0, 0, -0.5)  # Raise wing
        wing_r.keyframe_insert(data_path="rotation_euler", frame=20)
    if tail1:
        tail1.rotation_euler = (0, 0, 0.2)
        tail1.keyframe_insert(data_path="rotation_euler", frame=20)
    if tail2:
        tail2.rotation_euler = (0, 0, 0.2)
        tail2.keyframe_insert(data_path="rotation_euler", frame=20)
    
    # === Frame 35: Strike! (lunge forward, snap jaw) ===
    bpy.context.scene.frame_set(35)
    
    if head:
        head.rotation_euler = (-0.4, 0, 0)  # Lunge forward/down
        head.keyframe_insert(data_path="rotation_euler", frame=35)
    if neck:
        neck.rotation_euler = (0.3, 0, 0)  # Extend forward
        neck.keyframe_insert(data_path="rotation_euler", frame=35)
    if jaw:
        jaw.rotation_euler = (-0.1, 0, 0)  # Snap shut
        jaw.keyframe_insert(data_path="rotation_euler", frame=35)
    if wing_l:
        wing_l.rotation_euler = (0, 0, -0.2)  # Wings down
        wing_l.keyframe_insert(data_path="rotation_euler", frame=35)
    if wing_r:
        wing_r.rotation_euler = (0, 0, 0.2)  # Wings down
        wing_r.keyframe_insert(data_path="rotation_euler", frame=35)
    if tail1:
        tail1.rotation_euler = (0, 0, -0.3)
        tail1.keyframe_insert(data_path="rotation_euler", frame=35)
    if tail2:
        tail2.rotation_euler = (0, 0, -0.3)
        tail2.keyframe_insert(data_path="rotation_euler", frame=35)
    if tail3:
        tail3.rotation_euler = (0, 0, -0.2)
        tail3.keyframe_insert(data_path="rotation_euler", frame=35)
    
    # === Frame 50: Hold/impact ===
    bpy.context.scene.frame_set(50)
    
    if head:
        head.rotation_euler = (-0.3, 0, 0)
        head.keyframe_insert(data_path="rotation_euler", frame=50)
    if neck:
        neck.rotation_euler = (0.2, 0, 0)
        neck.keyframe_insert(data_path="rotation_euler", frame=50)
    if jaw:
        jaw.rotation_euler = (0, 0, 0)
        jaw.keyframe_insert(data_path="rotation_euler", frame=50)
    if wing_l:
        wing_l.rotation_euler = (0, 0, 0)
        wing_l.keyframe_insert(data_path="rotation_euler", frame=50)
    if wing_r:
        wing_r.rotation_euler = (0, 0, 0)
        wing_r.keyframe_insert(data_path="rotation_euler", frame=50)
    
    # === Frame 70: Return to neutral ===
    bpy.context.scene.frame_set(70)
    
    if head:
        head.rotation_euler = (0, 0, 0)
        head.keyframe_insert(data_path="rotation_euler", frame=70)
    if neck:
        neck.rotation_euler = (0, 0, 0)
        neck.keyframe_insert(data_path="rotation_euler", frame=70)
    if jaw:
        jaw.rotation_euler = (0, 0, 0)
        jaw.keyframe_insert(data_path="rotation_euler", frame=70)
    if wing_l:
        wing_l.rotation_euler = (0, 0, 0)
        wing_l.keyframe_insert(data_path="rotation_euler", frame=70)
    if wing_r:
        wing_r.rotation_euler = (0, 0, 0)
        wing_r.keyframe_insert(data_path="rotation_euler", frame=70)
    if tail1:
        tail1.rotation_euler = (0, 0, 0)
        tail1.keyframe_insert(data_path="rotation_euler", frame=70)
    if tail2:
        tail2.rotation_euler = (0, 0, 0)
        tail2.keyframe_insert(data_path="rotation_euler", frame=70)
    if tail3:
        tail3.rotation_euler = (0, 0, 0)
        tail3.keyframe_insert(data_path="rotation_euler", frame=70)
    
    bpy.ops.object.mode_set(mode='OBJECT')
    
    # Smooth interpolation
    for fcurve in action.fcurves:
        for keyframe in fcurve.keyframe_points:
            keyframe.interpolation = 'BEZIER'

def export_glb(filepath):
    """Export scene as GLB file"""
    bpy.ops.export_scene.gltf(
        filepath=filepath,
        export_format='GLB',
        export_animations=True,
        export_skins=True
    )
    print(f"Exported to: {filepath}")

def main():
    output_path = "C:/dev/nearest3/ai_art_team/character/3d_models/dragon_attack.glb"
    
    print("Creating dragon character with attack animation...")
    
    clear_scene()
    
    mesh_parts = create_dragon_mesh()
    print(f"Created {len(mesh_parts)} mesh parts")
    
    armature = create_dragon_armature()
    print("Created armature with 15 bones")
    
    character_mesh = parent_mesh_to_armature(mesh_parts, armature)
    print("Parented mesh to armature")
    
    create_attack_animation(armature)
    print("Created attack animation (70 frames)")
    
    export_glb(output_path)
    
    print("Done!")

if __name__ == "__main__":
    main()
