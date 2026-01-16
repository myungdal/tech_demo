"""
Blender Python Script: Dragon with DYNAMIC Attack Animation v2
- 더 과장된 움직임
- 전신 돌진
- 빠른 타이밍
"""

import bpy
import math

def clear_scene():
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete(use_global=False)

def create_material(name, color):
    mat = bpy.data.materials.new(name=name)
    mat.use_nodes = True
    bsdf = mat.node_tree.nodes["Principled BSDF"]
    bsdf.inputs['Base Color'].default_value = color
    return mat

def create_cube(name, location, scale, rotation=(0,0,0), material=None):
    bpy.ops.mesh.primitive_cube_add(size=1, location=location, rotation=rotation)
    obj = bpy.context.active_object
    obj.name = name
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
    if material:
        obj.data.materials.append(material)
    return obj

def create_cone(name, location, scale, rotation=(0,0,0), material=None):
    bpy.ops.mesh.primitive_cone_add(radius1=0.5, radius2=0, depth=1, location=location, rotation=rotation)
    obj = bpy.context.active_object
    obj.name = name
    obj.scale = scale
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
    if material:
        obj.data.materials.append(material)
    return obj

def create_dragon_mesh():
    """더 큰 드래곤"""
    body_mat = create_material("DragonBody", (0.15, 0.45, 0.15, 1.0))
    belly_mat = create_material("DragonBelly", (0.5, 0.6, 0.25, 1.0))
    wing_mat = create_material("DragonWing", (0.25, 0.5, 0.25, 1.0))
    horn_mat = create_material("DragonHorn", (0.25, 0.2, 0.15, 1.0))
    eye_mat = create_material("DragonEye", (1.0, 0.6, 0.0, 1.0))
    fire_mat = create_material("Fire", (1.0, 0.3, 0.0, 1.0))
    
    parts = []
    
    # BODY - 더 크게
    body = create_cube("Body", (0, 0, 1.0), (1.5, 0.7, 0.6), material=body_mat)
    parts.append(body)
    belly = create_cube("Belly", (0.1, 0, 0.8), (1.2, 0.5, 0.25), material=belly_mat)
    parts.append(belly)
    
    # HEAD - 더 크고 무서운
    head = create_cube("Head", (1.1, 0, 1.3), (0.5, 0.45, 0.45), material=body_mat)
    parts.append(head)
    snout = create_cube("Snout", (1.6, 0, 1.2), (0.5, 0.25, 0.25), material=body_mat)
    parts.append(snout)
    jaw = create_cube("Jaw", (1.5, 0, 1.05), (0.45, 0.22, 0.12), material=body_mat)
    parts.append(jaw)
    
    # 이빨
    for i in range(3):
        tooth_l = create_cone(f"Tooth_L{i}", (1.4 + i*0.15, 0.08, 1.0), (0.03, 0.03, 0.08),
                              rotation=(3.14, 0, 0), material=horn_mat)
        parts.append(tooth_l)
        tooth_r = create_cone(f"Tooth_R{i}", (1.4 + i*0.15, -0.08, 1.0), (0.03, 0.03, 0.08),
                              rotation=(3.14, 0, 0), material=horn_mat)
        parts.append(tooth_r)
    
    # Eyes
    eye_l = create_cube("Eye_L", (1.15, 0.2, 1.45), (0.1, 0.1, 0.1), material=eye_mat)
    parts.append(eye_l)
    eye_r = create_cube("Eye_R", (1.15, -0.2, 1.45), (0.1, 0.1, 0.1), material=eye_mat)
    parts.append(eye_r)
    
    # Horns - 더 크게
    horn_l = create_cone("Horn_L", (0.85, 0.2, 1.6), (0.1, 0.1, 0.35), 
                         rotation=(0.4, 0, 0.3), material=horn_mat)
    parts.append(horn_l)
    horn_r = create_cone("Horn_R", (0.85, -0.2, 1.6), (0.1, 0.1, 0.35), 
                         rotation=(-0.4, 0, -0.3), material=horn_mat)
    parts.append(horn_r)
    
    # NECK
    neck = create_cube("Neck", (0.7, 0, 1.2), (0.35, 0.3, 0.35), material=body_mat)
    parts.append(neck)
    
    # WINGS - 훨씬 더 크게
    wing_bone_l = create_cube("WingBone_L", (0, 0.6, 1.4), (0.12, 0.8, 0.1), 
                              rotation=(0, 0, 0.6), material=body_mat)
    parts.append(wing_bone_l)
    wing_bone_r = create_cube("WingBone_R", (0, -0.6, 1.4), (0.12, 0.8, 0.1), 
                              rotation=(0, 0, -0.6), material=body_mat)
    parts.append(wing_bone_r)
    
    wing_mem_l = create_cube("WingMem_L", (-0.3, 0.9, 1.25), (0.7, 0.5, 0.03), 
                             rotation=(0, 0, 0.4), material=wing_mat)
    parts.append(wing_mem_l)
    wing_mem_r = create_cube("WingMem_R", (-0.3, -0.9, 1.25), (0.7, 0.5, 0.03), 
                             rotation=(0, 0, -0.4), material=wing_mat)
    parts.append(wing_mem_r)
    
    # LEGS - 더 튼튼하게
    leg_fl = create_cube("Leg_FL", (0.6, 0.4, 0.45), (0.15, 0.15, 0.6), material=body_mat)
    parts.append(leg_fl)
    leg_fr = create_cube("Leg_FR", (0.6, -0.4, 0.45), (0.15, 0.15, 0.6), material=body_mat)
    parts.append(leg_fr)
    leg_bl = create_cube("Leg_BL", (-0.5, 0.45, 0.4), (0.18, 0.18, 0.65), material=body_mat)
    parts.append(leg_bl)
    leg_br = create_cube("Leg_BR", (-0.5, -0.45, 0.4), (0.18, 0.18, 0.65), material=body_mat)
    parts.append(leg_br)
    
    # Feet
    foot_fl = create_cube("Foot_FL", (0.65, 0.4, 0.1), (0.18, 0.18, 0.1), material=horn_mat)
    parts.append(foot_fl)
    foot_fr = create_cube("Foot_FR", (0.65, -0.4, 0.1), (0.18, 0.18, 0.1), material=horn_mat)
    parts.append(foot_fr)
    foot_bl = create_cube("Foot_BL", (-0.45, 0.45, 0.08), (0.22, 0.22, 0.1), material=horn_mat)
    parts.append(foot_bl)
    foot_br = create_cube("Foot_BR", (-0.45, -0.45, 0.08), (0.22, 0.22, 0.1), material=horn_mat)
    parts.append(foot_br)
    
    # TAIL - 더 길게
    tail1 = create_cube("Tail1", (-1.0, 0, 0.9), (0.45, 0.25, 0.25), material=body_mat)
    parts.append(tail1)
    tail2 = create_cube("Tail2", (-1.4, 0, 0.85), (0.4, 0.2, 0.2), material=body_mat)
    parts.append(tail2)
    tail3 = create_cube("Tail3", (-1.75, 0, 0.8), (0.35, 0.15, 0.15), material=body_mat)
    parts.append(tail3)
    tail4 = create_cube("Tail4", (-2.05, 0, 0.75), (0.3, 0.1, 0.1), material=body_mat)
    parts.append(tail4)
    tail_spike = create_cone("TailSpike", (-2.3, 0, 0.7), (0.12, 0.12, 0.25), 
                             rotation=(0, -1.57, 0), material=horn_mat)
    parts.append(tail_spike)
    
    return parts

def create_dragon_armature():
    bpy.ops.object.armature_add(enter_editmode=True, location=(0, 0, 0))
    armature = bpy.context.active_object
    armature.name = "DragonArmature"
    arm_data = armature.data
    arm_data.name = "DragonRig"
    
    bpy.ops.armature.select_all(action='SELECT')
    bpy.ops.armature.delete()
    
    bones_info = {
        'root': {'head': (0, 0, 1.0), 'tail': (-0.3, 0, 0.8), 'parent': None},
        'spine': {'head': (0, 0, 1.0), 'tail': (0.5, 0, 1.1), 'parent': 'root'},
        'chest': {'head': (0.5, 0, 1.1), 'tail': (0.7, 0, 1.2), 'parent': 'spine'},
        'neck': {'head': (0.7, 0, 1.2), 'tail': (1.0, 0, 1.3), 'parent': 'chest'},
        'head': {'head': (1.0, 0, 1.3), 'tail': (1.5, 0, 1.2), 'parent': 'neck'},
        'jaw': {'head': (1.2, 0, 1.1), 'tail': (1.6, 0, 1.0), 'parent': 'head'},
        
        'wing_L': {'head': (0.2, 0.35, 1.3), 'tail': (0, 1.0, 1.5), 'parent': 'chest'},
        'wing_R': {'head': (0.2, -0.35, 1.3), 'tail': (0, -1.0, 1.5), 'parent': 'chest'},
        
        'leg_FL': {'head': (0.6, 0.4, 0.75), 'tail': (0.65, 0.4, 0.1), 'parent': 'spine'},
        'leg_FR': {'head': (0.6, -0.4, 0.75), 'tail': (0.65, -0.4, 0.1), 'parent': 'spine'},
        'leg_BL': {'head': (-0.5, 0.45, 0.7), 'tail': (-0.45, 0.45, 0.08), 'parent': 'root'},
        'leg_BR': {'head': (-0.5, -0.45, 0.7), 'tail': (-0.45, -0.45, 0.08), 'parent': 'root'},
        
        'tail1': {'head': (-0.75, 0, 0.9), 'tail': (-1.2, 0, 0.85), 'parent': 'root'},
        'tail2': {'head': (-1.2, 0, 0.85), 'tail': (-1.6, 0, 0.8), 'parent': 'tail1'},
        'tail3': {'head': (-1.6, 0, 0.8), 'tail': (-1.95, 0, 0.75), 'parent': 'tail2'},
        'tail4': {'head': (-1.95, 0, 0.75), 'tail': (-2.3, 0, 0.7), 'parent': 'tail3'},
    }
    
    created_bones = {}
    for bone_name, info in bones_info.items():
        bone = arm_data.edit_bones.new(bone_name)
        bone.head = info['head']
        bone.tail = info['tail']
        created_bones[bone_name] = bone
    
    for bone_name, info in bones_info.items():
        if info['parent']:
            created_bones[bone_name].parent = created_bones[info['parent']]
    
    bpy.ops.object.mode_set(mode='OBJECT')
    return armature

def parent_mesh_to_armature(mesh_parts, armature):
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
    """역동적인 공격 애니메이션"""
    bpy.context.scene.frame_start = 1
    bpy.context.scene.frame_end = 70
    bpy.context.scene.render.fps = 10
    
    action = bpy.data.actions.new(name="Attack")
    armature.animation_data_create()
    armature.animation_data.action = action
    
    bpy.context.view_layer.objects.active = armature
    bpy.ops.object.mode_set(mode='POSE')
    
    # 뼈대 가져오기
    root = armature.pose.bones.get('root')
    spine = armature.pose.bones.get('spine')
    chest = armature.pose.bones.get('chest')
    neck = armature.pose.bones.get('neck')
    head = armature.pose.bones.get('head')
    jaw = armature.pose.bones.get('jaw')
    wing_l = armature.pose.bones.get('wing_L')
    wing_r = armature.pose.bones.get('wing_R')
    leg_fl = armature.pose.bones.get('leg_FL')
    leg_fr = armature.pose.bones.get('leg_FR')
    leg_bl = armature.pose.bones.get('leg_BL')
    leg_br = armature.pose.bones.get('leg_BR')
    tail1 = armature.pose.bones.get('tail1')
    tail2 = armature.pose.bones.get('tail2')
    tail3 = armature.pose.bones.get('tail3')
    tail4 = armature.pose.bones.get('tail4')
    
    def keyframe_all(frame):
        """모든 뼈대 키프레임"""
        bones = [root, spine, chest, neck, head, jaw, wing_l, wing_r, 
                 leg_fl, leg_fr, leg_bl, leg_br, tail1, tail2, tail3, tail4]
        for b in bones:
            if b:
                b.keyframe_insert(data_path="rotation_euler", frame=frame)
                b.keyframe_insert(data_path="location", frame=frame)
    
    # === FRAME 1: 대기 자세 ===
    bpy.context.scene.frame_set(1)
    if root: root.location = (0, 0, 0); root.rotation_euler = (0, 0, 0)
    if spine: spine.rotation_euler = (0, 0, 0)
    if chest: chest.rotation_euler = (0, 0, 0)
    if neck: neck.rotation_euler = (0.1, 0, 0)  # 약간 위를 봄
    if head: head.rotation_euler = (0, 0, 0)
    if jaw: jaw.rotation_euler = (0, 0, 0)
    if wing_l: wing_l.rotation_euler = (0, 0, 0)
    if wing_r: wing_r.rotation_euler = (0, 0, 0)
    if tail1: tail1.rotation_euler = (0, 0, 0)
    if tail2: tail2.rotation_euler = (0, 0, 0)
    if tail3: tail3.rotation_euler = (0, 0, 0)
    if tail4: tail4.rotation_euler = (0, 0, 0)
    keyframe_all(1)
    
    # === FRAME 15: 준비 동작 (뒤로 물러서며 몸 웅크림) ===
    bpy.context.scene.frame_set(15)
    if root: root.location = (-0.3, 0, -0.1); root.rotation_euler = (-0.2, 0, 0)  # 뒤로, 아래로
    if spine: spine.rotation_euler = (-0.3, 0, 0)  # 웅크림
    if chest: chest.rotation_euler = (-0.2, 0, 0)
    if neck: neck.rotation_euler = (-0.4, 0, 0)  # 머리 뒤로 당김
    if head: head.rotation_euler = (0.5, 0, 0)  # 위를 봄
    if jaw: jaw.rotation_euler = (0.8, 0, 0)  # 입 크게 벌림!
    if wing_l: wing_l.rotation_euler = (0.3, 0.2, 0.8)  # 날개 크게 올림
    if wing_r: wing_r.rotation_euler = (0.3, -0.2, -0.8)
    if leg_fl: leg_fl.rotation_euler = (0.3, 0, 0)  # 앞다리 들어올림
    if leg_fr: leg_fr.rotation_euler = (0.3, 0, 0)
    if leg_bl: leg_bl.rotation_euler = (-0.2, 0, 0)  # 뒷다리 힘 모음
    if leg_br: leg_br.rotation_euler = (-0.2, 0, 0)
    if tail1: tail1.rotation_euler = (0.2, 0, 0.3)  # 꼬리 들어올림
    if tail2: tail2.rotation_euler = (0.1, 0, 0.4)
    if tail3: tail3.rotation_euler = (0.1, 0, 0.5)
    if tail4: tail4.rotation_euler = (0, 0, 0.6)
    keyframe_all(15)
    
    # === FRAME 25: 폭발적 돌진! ===
    bpy.context.scene.frame_set(25)
    if root: root.location = (0.5, 0, 0.15); root.rotation_euler = (0.3, 0, 0)  # 앞으로 돌진!
    if spine: spine.rotation_euler = (0.4, 0, 0)  # 몸 펴짐
    if chest: chest.rotation_euler = (0.3, 0, 0)
    if neck: neck.rotation_euler = (0.5, 0, 0)  # 목 앞으로 쭉!
    if head: head.rotation_euler = (-0.6, 0, 0)  # 머리 아래로 내려치기
    if jaw: jaw.rotation_euler = (-0.2, 0, 0)  # 입 다물기 (물기!)
    if wing_l: wing_l.rotation_euler = (-0.3, 0.1, -0.4)  # 날개 아래로 휙!
    if wing_r: wing_r.rotation_euler = (-0.3, -0.1, 0.4)
    if leg_fl: leg_fl.rotation_euler = (-0.4, 0, 0)  # 앞다리 땅 짚기
    if leg_fr: leg_fr.rotation_euler = (-0.4, 0, 0)
    if leg_bl: leg_bl.rotation_euler = (0.3, 0, 0)  # 뒷다리 밀기
    if leg_br: leg_br.rotation_euler = (0.3, 0, 0)
    if tail1: tail1.rotation_euler = (-0.1, 0, -0.4)  # 꼬리 반대방향으로 휙!
    if tail2: tail2.rotation_euler = (-0.1, 0, -0.5)
    if tail3: tail3.rotation_euler = (-0.1, 0, -0.6)
    if tail4: tail4.rotation_euler = (-0.1, 0, -0.7)
    keyframe_all(25)
    
    # === FRAME 35: 물어뜯기 유지 ===
    bpy.context.scene.frame_set(35)
    if root: root.location = (0.4, 0, 0.1); root.rotation_euler = (0.2, 0, 0)
    if spine: spine.rotation_euler = (0.3, 0, 0)
    if chest: chest.rotation_euler = (0.2, 0, 0)
    if neck: neck.rotation_euler = (0.4, 0, 0)
    if head: head.rotation_euler = (-0.4, 0, 0.1)  # 고개 살짝 틀기 (뜯기)
    if jaw: jaw.rotation_euler = (0.1, 0, 0)  # 다시 벌림
    if wing_l: wing_l.rotation_euler = (0, 0, 0.2)
    if wing_r: wing_r.rotation_euler = (0, 0, -0.2)
    if tail1: tail1.rotation_euler = (0, 0, -0.2)
    if tail2: tail2.rotation_euler = (0, 0, -0.3)
    if tail3: tail3.rotation_euler = (0, 0, -0.3)
    if tail4: tail4.rotation_euler = (0, 0, -0.3)
    keyframe_all(35)
    
    # === FRAME 50: 뒤로 물러남 ===
    bpy.context.scene.frame_set(50)
    if root: root.location = (-0.1, 0, 0); root.rotation_euler = (-0.1, 0, 0)
    if spine: spine.rotation_euler = (-0.1, 0, 0)
    if chest: chest.rotation_euler = (0, 0, 0)
    if neck: neck.rotation_euler = (0.2, 0, 0)
    if head: head.rotation_euler = (0.1, 0, 0)
    if jaw: jaw.rotation_euler = (0, 0, 0)
    if wing_l: wing_l.rotation_euler = (0, 0, 0.1)
    if wing_r: wing_r.rotation_euler = (0, 0, -0.1)
    if tail1: tail1.rotation_euler = (0, 0, 0.1)
    if tail2: tail2.rotation_euler = (0, 0, 0.1)
    if tail3: tail3.rotation_euler = (0, 0, 0.1)
    if tail4: tail4.rotation_euler = (0, 0, 0.1)
    keyframe_all(50)
    
    # === FRAME 70: 원위치 ===
    bpy.context.scene.frame_set(70)
    if root: root.location = (0, 0, 0); root.rotation_euler = (0, 0, 0)
    if spine: spine.rotation_euler = (0, 0, 0)
    if chest: chest.rotation_euler = (0, 0, 0)
    if neck: neck.rotation_euler = (0.1, 0, 0)
    if head: head.rotation_euler = (0, 0, 0)
    if jaw: jaw.rotation_euler = (0, 0, 0)
    if wing_l: wing_l.rotation_euler = (0, 0, 0)
    if wing_r: wing_r.rotation_euler = (0, 0, 0)
    if leg_fl: leg_fl.rotation_euler = (0, 0, 0)
    if leg_fr: leg_fr.rotation_euler = (0, 0, 0)
    if leg_bl: leg_bl.rotation_euler = (0, 0, 0)
    if leg_br: leg_br.rotation_euler = (0, 0, 0)
    if tail1: tail1.rotation_euler = (0, 0, 0)
    if tail2: tail2.rotation_euler = (0, 0, 0)
    if tail3: tail3.rotation_euler = (0, 0, 0)
    if tail4: tail4.rotation_euler = (0, 0, 0)
    keyframe_all(70)
    
    bpy.ops.object.mode_set(mode='OBJECT')
    
    # 베지어 보간
    for fcurve in action.fcurves:
        for keyframe in fcurve.keyframe_points:
            keyframe.interpolation = 'BEZIER'
            keyframe.handle_left_type = 'AUTO_CLAMPED'
            keyframe.handle_right_type = 'AUTO_CLAMPED'

def export_glb(filepath):
    bpy.ops.export_scene.gltf(
        filepath=filepath,
        export_format='GLB',
        export_animations=True,
        export_skins=True
    )
    print(f"Exported to: {filepath}")

def main():
    output_path = "C:/dev/nearest3/ai_art_team/character/3d_models/dragon_attack_v2.glb"
    
    print("Creating DYNAMIC dragon attack animation v2...")
    clear_scene()
    
    mesh_parts = create_dragon_mesh()
    print(f"Created {len(mesh_parts)} mesh parts")
    
    armature = create_dragon_armature()
    print("Created armature")
    
    character_mesh = parent_mesh_to_armature(mesh_parts, armature)
    print("Parented mesh")
    
    create_attack_animation(armature)
    print("Created DYNAMIC attack animation!")
    
    export_glb(output_path)
    print("Done!")

if __name__ == "__main__":
    main()
