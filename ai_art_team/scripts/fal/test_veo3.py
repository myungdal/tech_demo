"""
Google Veo 3로 구르기 애니메이션 테스트
"""

import fal_client
import os
import requests
from PIL import Image

# API Key
with open("C:/dev/nearest3/secret/txt/fal.txt", "r") as f:
    api_key = f.read().strip()
os.environ["FAL_KEY"] = api_key

OUTPUT_DIR = "C:/dev/nearest3/ai_art_team/character/animation_test/warrior"
CHARACTER_IMAGE = "C:/dev/nearest3/ai_art_team/character/individual/01.png"

def upscale_image(image_path: str, scale: int = 8) -> str:
    img = Image.open(image_path)
    new_size = (img.width * scale, img.height * scale)
    upscaled = img.resize(new_size, Image.Resampling.NEAREST)
    temp_path = os.path.join(OUTPUT_DIR, "upscaled_8x.png")
    upscaled.save(temp_path)
    return temp_path

def test_veo3():
    print("="*60)
    print("Model: Google Veo 3")
    print("="*60)
    
    # 이미지 준비
    upscaled_path = upscale_image(CHARACTER_IMAGE)
    image_url = fal_client.upload_file(upscaled_path)
    print(f"Uploaded: {image_url[:50]}...")
    
    # 프롬프트 - 땅에서 구르는 것 강조
    prompt = "pixel art warrior rolling on the ground, dodge roll on floor, tumbling forward on solid ground, grounded rolling motion, not floating, touching the ground while rolling"
    
    print(f"Prompt: {prompt}")
    print("Generating... (may take 2-5 minutes)")
    
    try:
        result = fal_client.subscribe(
            "fal-ai/veo3",
            arguments={
                "image_url": image_url,
                "prompt": prompt,
                "aspect_ratio": "9:16",  # 세로 비율 (1:1 미지원)
            },
            with_logs=True,
        )
        
        print(f"\nResult keys: {result.keys()}")
        print(f"Result: {result}")
        
        # 비디오 URL 찾기
        video_url = None
        if "video" in result:
            if isinstance(result["video"], dict) and "url" in result["video"]:
                video_url = result["video"]["url"]
            elif isinstance(result["video"], str):
                video_url = result["video"]
        
        if video_url:
            output_path = os.path.join(OUTPUT_DIR, "roll_veo3.mp4")
            response = requests.get(video_url)
            with open(output_path, "wb") as f:
                f.write(response.content)
            print(f"OK Saved: {output_path}")
            return output_path
        else:
            print("FAIL: No video URL")
            return None
            
    except Exception as e:
        print(f"FAIL: {e}")
        return None

if __name__ == "__main__":
    test_veo3()
