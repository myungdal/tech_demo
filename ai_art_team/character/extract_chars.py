import os
import sys
from PIL import Image

# 경로 설정
INPUT_PATH = os.path.join('ai_art_team', '캐릭터', '썸네일', '12cards.png')
OUTPUT_DIR = os.path.join('ai_art_team', '캐릭터', '추출본')

# 출력 크기
TARGET_SIZE = (100, 100)

# 캐릭터 매핑 (이미지상의 순서: 좌->우, 위->아래)
# Row 1: Warrior (Roman Soldier), Archer, Assassin (Rogue), Bat
# Row 2: Bear, Spider, Zeus, Hades (Poseidon)
# Row 3: Cupid, Golem, Phoenix, Dragon
CHARACTERS = [
    'warrior', 'archer', 'assassin', 'bat',
    'bear', 'spider', 'zeus', 'hades',
    'cupid', 'golem', 'phoenix', 'dragon'
]

GRID_COLS = 4
GRID_ROWS = 3

def is_background_pixel(pixel, tolerance=30):
    """배경색 (베이지/크림색 계열) 또는 프레임 색상인지 판별"""
    r, g, b = pixel[:3]
    alpha = pixel[3] if len(pixel) > 3 else 255
    
    # 이미 투명한 픽셀
    if alpha < 10:
        return True
    
    # 베이지/크림색 배경 (카드 배경)
    if r > 200 and g > 180 and b > 140:
        return True
    
    # 갈색 프레임
    if 60 < r < 120 and 30 < g < 80 and 10 < b < 60:
        return True
    
    # 체크무늬 배경 (투명 표시용)
    if (r > 230 and g > 230 and b > 230) or (180 < r < 210 and 180 < g < 210 and 180 < b < 210):
        return True
    
    return False

def remove_background(img):
    """배경을 투명하게 처리"""
    data = img.getdata()
    new_data = []
    
    for pixel in data:
        if is_background_pixel(pixel):
            new_data.append((0, 0, 0, 0))  # 완전 투명
        else:
            new_data.append(pixel)
    
    img.putdata(new_data)
    return img

def extract_character_from_card(card_img):
    """카드에서 캐릭터 영역만 추출 (텍스트 영역 제외)"""
    width, height = card_img.size
    
    # 카드 하단의 텍스트 영역 제외 (약 20~25% 정도가 텍스트)
    # 그리고 프레임 영역도 제외 (좌우상하 약 5~10% 정도)
    margin_x = int(width * 0.08)
    margin_top = int(height * 0.05)
    margin_bottom = int(height * 0.22)  # 하단 텍스트 영역
    
    # 캐릭터 영역만 크롭
    char_region = card_img.crop((margin_x, margin_top, width - margin_x, height - margin_bottom))
    
    return char_region

def flip_if_facing_left(img, char_name):
    """캐릭터가 왼쪽을 바라보고 있으면 오른쪽으로 플립"""
    # 이 이미지에서 대부분의 캐릭터가 이미 오른쪽을 보고 있음
    # 하지만 일부 캐릭터는 왼쪽을 보고 있을 수 있으니 확인 필요
    # 현재 카드 이미지에서는 모든 캐릭터가 오른쪽을 보고 있음
    return img

def process_images():
    if not os.path.exists(INPUT_PATH):
        print(f"Error: 파일이 없습니다 - {INPUT_PATH}")
        return False

    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    print(f"이미지 로드 중: {INPUT_PATH}")
    try:
        img = Image.open(INPUT_PATH).convert("RGBA")
    except Exception as e:
        print(f"이미지 열기 실패: {e}")
        return False

    width, height = img.size
    print(f"이미지 크기: {width} x {height}")
    
    # 그리드 셀 크기 계산
    cell_w = width // GRID_COLS
    cell_h = height // GRID_ROWS
    print(f"셀 크기: {cell_w} x {cell_h}")

    count = 0
    for row in range(GRID_ROWS):
        for col in range(GRID_COLS):
            if count >= len(CHARACTERS):
                break
            
            name = CHARACTERS[count]
            
            # 1. 격자로 카드 자르기
            left = col * cell_w
            top = row * cell_h
            right = left + cell_w
            bottom = top + cell_h
            
            card_img = img.crop((left, top, right, bottom))
            
            # 2. 카드에서 캐릭터 영역만 추출 (텍스트 제외)
            char_img = extract_character_from_card(card_img)
            
            # 3. 배경 제거 (투명화)
            char_img = remove_background(char_img)
            
            # 4. 여백 잘라내기 (Bounding Box)
            bbox = char_img.getbbox()
            if bbox:
                char_img = char_img.crop(bbox)
            else:
                print(f"⚠️ 경고: {name} 캐릭터의 바운딩박스를 찾을 수 없습니다")
                count += 1
                continue
            
            # 5. 오른쪽을 바라보도록 플립 (필요시)
            char_img = flip_if_facing_left(char_img, name)
            
            # 6. 100x100 캔버스 중앙 정렬
            final_img = Image.new("RGBA", TARGET_SIZE, (0, 0, 0, 0))
            
            # 원본 비율 유지하면서 최대 크기로 맞춤 (Pixel Art니까 NEAREST 사용)
            max_dim = 90  # 여백 5px씩
            w, h = char_img.size
            ratio = min(max_dim / w, max_dim / h)
            
            # 확대/축소
            new_w = max(1, int(w * ratio))
            new_h = max(1, int(h * ratio))
            
            char_img_resized = char_img.resize((new_w, new_h), Image.Resampling.NEAREST)
            
            # 중앙 배치
            offset_x = (TARGET_SIZE[0] - new_w) // 2
            offset_y = (TARGET_SIZE[1] - new_h) // 2
            
            final_img.paste(char_img_resized, (offset_x, offset_y), char_img_resized)
            
            # 저장
            save_path = os.path.join(OUTPUT_DIR, f"{name}.png")
            final_img.save(save_path)
            print(f"✅ 저장됨: {name}.png ({new_w}x{new_h})")
            
            count += 1
    
    print(f"\n완료: {count}개 캐릭터 추출됨")
    return True

if __name__ == "__main__":
    process_images()
