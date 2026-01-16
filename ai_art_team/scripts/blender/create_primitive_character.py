"""
Blender Python Script: Create Primitive Character with Idle Animation
Usage: blender --background --python create_primitive_character.py -- --output warrior.glb
"""

import bpy
import math
import sys
import os

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

def create_cube(name, location, scale, material=None):
    """Create a cube with given parameters"""
    bpy.ops.mesh.primitive_cube_add(size=1, location=location)
    obj = bpy.context.active_object
    obj.name = name
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    
    if material:
        obj.data.materials.append(material)
    
    return obj

def create_warrior_mesh():
    """Create warrior character from primitive cubes"""
    # Materials
    body_mat = create_material("BodyMat", (0.4, 0.3, 0.2, 1.0))  # Brown armor
    skin_mat = create_material("SkinMat", (0.9, 0.7, 0.5, 1.0))  # Skin tone
    sword_mat = create_material("SwordMat", (0.7, 0.7, 0.8, 1.0))  # Metal
    
    parts = []
    
    # Body (torso)
    body = create_cube("Body", (0, 0, 1.0), (0.5, 0.3, 0.6), body_mat)
    parts.append(body)
    
    # Head
    head = create_cube("Head", (0, 0, 1.6), (0.35, 0.35, 0.35), skin_mat)
    parts.append(head)
    
    # Left Arm
    arm_l = create_cube("Arm_L", (0.45, 0, 1.0), (0.15, 0.15, 0.5), skin_mat)
    parts.append(arm_l)
    
    # Right Arm
    arm_r = create_cube("Arm_R", (-0.45, 0, 1.0), (0.15, 0.15, 0.5), skin_mat)
    parts.append(arm_r)
    
    # Left Leg
    leg_l = create_cube("Leg_L", (0.2, 0, 0.35), (0.18, 0.18, 0.5), body_mat)
    parts.append(leg_l)
    
    # Right Leg
    leg_r = create_cube("Leg_R", (-0.2, 0, 0.35), (0.18, 0.18, 0.5), body_mat)
    parts.append(leg_r)
    
    # Sword (attached to right arm)
    sword = create_cube("Sword", (-0.45, 0, 0.4), (0.05, 0.05, 0.6), sword_mat)
    parts.append(sword)
    
    return parts

def create_armature():
    """Create armature (skeleton) for the character"""
    # Create armature
    bpy.ops.object.armature_add(enter_editmode=True, location=(0, 0, 0))
    armature = bpy.context.active_object
    armature.name = "Armature"
    
    # Get armature data
    arm_data = armature.data
    arm_data.name = "CharacterRig"
    
    # Clear default bone
    bpy.ops.armature.select_all(action='SELECT')
    bpy.ops.armature.delete()
    
    # Create bones
    bones_info = {
        'root': {'head': (0, 0, 0), 'tail': (0, 0, 0.3), 'parent': None},
        'spine': {'head': (0, 0, 0.7), 'tail': (0, 0, 1.3), 'parent': 'root'},
        'head': {'head': (0, 0, 1.3), 'tail': (0, 0, 1.8), 'parent': 'spine'},
        'arm_L': {'head': (0.35, 0, 1.2), 'tail': (0.55, 0, 0.8), 'parent': 'spine'},
        'arm_R': {'head': (-0.35, 0, 1.2), 'tail': (-0.55, 0, 0.8), 'parent': 'spine'},
        'leg_L': {'head': (0.2, 0, 0.6), 'tail': (0.2, 0, 0.1), 'parent': 'root'},
        'leg_R': {'head': (-0.2, 0, 0.6), 'tail': (-0.2, 0, 0.1), 'parent': 'root'},
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
    # Join all mesh parts into one
    bpy.ops.object.select_all(action='DESELECT')
    
    for part in mesh_parts:
        part.select_set(True)
    
    bpy.context.view_layer.objects.active = mesh_parts[0]
    bpy.ops.object.join()
    
    merged_mesh = bpy.context.active_object
    merged_mesh.name = "Character"
    
    # Parent to armature with automatic weights
    bpy.ops.object.select_all(action='DESELECT')
    merged_mesh.select_set(True)
    armature.select_set(True)
    bpy.context.view_layer.objects.active = armature
    
    bpy.ops.object.parent_set(type='ARMATURE_AUTO')
    
    return merged_mesh

def create_idle_animation(armature):
    """Create simple idle animation (breathing motion)"""
    # Set up animation
    bpy.context.scene.frame_start = 1
    bpy.context.scene.frame_end = 60
    bpy.context.scene.render.fps = 10
    
    # Create action
    action = bpy.data.actions.new(name="Idle")
    armature.animation_data_create()
    armature.animation_data.action = action
    
    # Go to pose mode
    bpy.context.view_layer.objects.active = armature
    bpy.ops.object.mode_set(mode='POSE')
    
    spine_bone = armature.pose.bones.get('spine')
    arm_l_bone = armature.pose.bones.get('arm_L')
    arm_r_bone = armature.pose.bones.get('arm_R')
    
    # Keyframe 1: Neutral pose
    bpy.context.scene.frame_set(1)
    if spine_bone:
        spine_bone.scale = (1.0, 1.0, 1.0)
        spine_bone.keyframe_insert(data_path="scale", frame=1)
    
    if arm_l_bone:
        arm_l_bone.rotation_euler = (0, 0, 0)
        arm_l_bone.keyframe_insert(data_path="rotation_euler", frame=1)
    
    if arm_r_bone:
        arm_r_bone.rotation_euler = (0, 0, 0)
        arm_r_bone.keyframe_insert(data_path="rotation_euler", frame=1)
    
    # Keyframe 2: Breathe in (slight expansion)
    bpy.context.scene.frame_set(30)
    if spine_bone:
        spine_bone.scale = (1.02, 1.02, 1.03)
        spine_bone.keyframe_insert(data_path="scale", frame=30)
    
    if arm_l_bone:
        arm_l_bone.rotation_euler = (0, 0, 0.05)
        arm_l_bone.keyframe_insert(data_path="rotation_euler", frame=30)
    
    if arm_r_bone:
        arm_r_bone.rotation_euler = (0, 0, -0.05)
        arm_r_bone.keyframe_insert(data_path="rotation_euler", frame=30)
    
    # Keyframe 3: Back to neutral (end = loop)
    bpy.context.scene.frame_set(60)
    if spine_bone:
        spine_bone.scale = (1.0, 1.0, 1.0)
        spine_bone.keyframe_insert(data_path="scale", frame=60)
    
    if arm_l_bone:
        arm_l_bone.rotation_euler = (0, 0, 0)
        arm_l_bone.keyframe_insert(data_path="rotation_euler", frame=60)
    
    if arm_r_bone:
        arm_r_bone.rotation_euler = (0, 0, 0)
        arm_r_bone.keyframe_insert(data_path="rotation_euler", frame=60)
    
    bpy.ops.object.mode_set(mode='OBJECT')
    
    # Set interpolation to smooth
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
    # Parse command line arguments
    argv = sys.argv
    if "--" in argv:
        argv = argv[argv.index("--") + 1:]
    else:
        argv = []
    
    # Default output path
    output_path = "C:/dev/nearest3/ai_art_team/character/3d_models/warrior_primitive.glb"
    
    for i, arg in enumerate(argv):
        if arg == "--output" and i + 1 < len(argv):
            output_path = argv[i + 1]
    
    print("Creating primitive warrior character...")
    
    # Clear scene
    clear_scene()
    
    # Create mesh parts
    mesh_parts = create_warrior_mesh()
    print(f"Created {len(mesh_parts)} mesh parts")
    
    # Create armature
    armature = create_armature()
    print("Created armature")
    
    # Parent mesh to armature
    character_mesh = parent_mesh_to_armature(mesh_parts, armature)
    print("Parented mesh to armature")
    
    # Create idle animation
    create_idle_animation(armature)
    print("Created idle animation")
    
    # Export
    export_glb(output_path)
    
    print("Done!")

if __name__ == "__main__":
    main()
