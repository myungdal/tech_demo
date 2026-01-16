"""
동영상 프레임 → 스프라이트 시트 변환
"""

from PIL import Image
import os
import math

def remove_white_background(img, threshold=240):
    """흰색 배경을 투명하게 변환"""
    img = img.convert('RGBA')
    data = img.getdata()
    
    new_data = []
    for item in data:
        # 흰색에 가까운 픽셀을 투명하게
        if item[0] > threshold and item[1] > threshold and item[2] > threshold:
            new_data.append((255, 255, 255, 0))  # 투명
        else:
            new_data.append(item)
    
    img.putdata(new_data)
    return img

def create_sprite_sheet(
    frames_dir: str,
    output_path: str,
    target_size: int = 128,
    max_frames: int = 8,
    cols: int = 8
):
    """
    프레임들을 스프라이트 시트로 합침
    
    Args:
        frames_dir: 프레임 이미지들이 있는 폴더
        output_path: 출력 스프라이트 시트 경로
        target_size: 각 프레임의 목표 크기 (정사각형)
        max_frames: 사용할 최대 프레임 수
        cols: 열 개수
    """
    # 프레임 파일 목록
    frame_files = sorted([f for f in os.listdir(frames_dir) if f.endswith('.png')])
    print(f"Found {len(frame_files)} frames")
    
    # 프레임 수 제한 (균등하게 샘플링)
    if len(frame_files) > max_frames:
        step = len(frame_files) / max_frames
        selected_indices = [int(i * step) for i in range(max_frames)]
        frame_files = [frame_files[i] for i in selected_indices]
        print(f"Selected {len(frame_files)} frames (sampled)")
    
    num_frames = len(frame_files)
    rows = math.ceil(num_frames / cols)
    
    # 스프라이트 시트 생성
    sheet_width = cols * target_size
    sheet_height = rows * target_size
    sprite_sheet = Image.new('RGBA', (sheet_width, sheet_height), (0, 0, 0, 0))
    
    print(f"Creating {sheet_width}x{sheet_height} sprite sheet ({cols}x{rows} grid)")
    
    for i, frame_file in enumerate(frame_files):
        frame_path = os.path.join(frames_dir, frame_file)
        frame = Image.open(frame_path).convert('RGBA')
        
        # 흰색 배경 제거
        frame = remove_white_background(frame)
        
        # 다운스케일 (NEAREST로 픽셀아트 스타일 유지)
        frame_resized = frame.resize((target_size, target_size), Image.Resampling.NEAREST)
        
        # 위치 계산
        col = i % cols
        row = i // cols
        x = col * target_size
        y = row * target_size
        
        sprite_sheet.paste(frame_resized, (x, y))
        print(f"  Frame {i+1}: {frame_file} -> ({col}, {row})")
    
    # 저장
    sprite_sheet.save(output_path)
    print(f"\nSaved: {output_path}")
    print(f"Size: {sheet_width}x{sheet_height}, {num_frames} frames")
    
    return sprite_sheet

def main():
    frames_dir = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack_frames"
    
    # 128x128 버전 (8프레임)
    output_128 = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack_spritesheet_128.png"
    create_sprite_sheet(frames_dir, output_128, target_size=128, max_frames=8, cols=8)
    
    # 64x64 버전 (8프레임)  
    output_64 = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior_attack_spritesheet_64.png"
    create_sprite_sheet(frames_dir, output_64, target_size=64, max_frames=8, cols=8)
    
    print("\n=== Done! ===")

if __name__ == "__main__":
    main()
