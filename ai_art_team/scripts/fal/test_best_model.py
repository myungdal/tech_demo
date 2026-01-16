"""
fal.ai 최고 품질 모델로 구르기 애니메이션 테스트
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

# 테스트할 모델들 (품질순)
MODELS = [
    "fal-ai/kling-video/v1.6/pro/image-to-video",      # Kling 1.6 Pro (최신)
    "fal-ai/kling-video/v1.5/pro/image-to-video",      # Kling 1.5 Pro
    "fal-ai/minimax-video/image-to-video",             # MiniMax
    "fal-ai/luma-dream-machine/image-to-video",        # Luma
]

def upscale_image(image_path: str, scale: int = 8) -> str:
    img = Image.open(image_path)
    new_size = (img.width * scale, img.height * scale)
    upscaled = img.resize(new_size, Image.Resampling.NEAREST)
    temp_path = os.path.join(OUTPUT_DIR, "upscaled_8x.png")
    upscaled.save(temp_path)
    return temp_path

def test_model(model: str, prompt: str, output_name: str):
    print(f"\n{'='*60}")
    print(f"Model: {model}")
    print(f"Prompt: {prompt}")
    print("="*60)
    
    # 이미지 준비
    upscaled_path = upscale_image(CHARACTER_IMAGE)
    image_url = fal_client.upload_file(upscaled_path)
    print(f"Uploaded image: {image_url[:50]}...")
    
    try:
        # 모델별 파라미터
        if "kling" in model:
            args = {
                "image_url": image_url,
                "prompt": prompt,
                "duration": "5",        # 5초
                "aspect_ratio": "1:1",
            }
        elif "minimax" in model:
            args = {
                "image_url": image_url,
                "prompt": prompt,
            }
        elif "luma" in model:
            args = {
                "image_url": image_url,
                "prompt": prompt,
                "aspect_ratio": "1:1",
            }
        else:
            args = {
                "image_url": image_url,
                "prompt": prompt,
            }
        
        print(f"Generating... (this may take 2-5 minutes)")
        
        result = fal_client.subscribe(
            model,
            arguments=args,
            with_logs=True,
        )
        
        print(f"\nResult keys: {result.keys()}")
        
        # 비디오 URL 찾기
        video_url = None
        if "video" in result:
            if isinstance(result["video"], dict) and "url" in result["video"]:
                video_url = result["video"]["url"]
            elif isinstance(result["video"], str):
                video_url = result["video"]
        
        if video_url:
            output_path = os.path.join(OUTPUT_DIR, f"{output_name}.mp4")
            response = requests.get(video_url)
            with open(output_path, "wb") as f:
                f.write(response.content)
            print(f"OK Saved: {output_path}")
            return output_path
        else:
            print(f"FAIL: No video URL in result")
            print(f"Result: {result}")
            return None
            
    except Exception as e:
        print(f"FAIL: {e}")
        return None

def main():
    # 구르기 프롬프트 - 매우 강조
    prompt = "pixel art warrior character doing a forward roll, tumbling roll animation, somersault dodge roll, dynamic rolling motion on ground, full body rotation, action game roll dodge"
    
    # Kling 1.6 Pro 테스트 (가장 좋은 모델)
    test_model(
        model="fal-ai/kling-video/v1.6/pro/image-to-video",
        prompt=prompt,
        output_name="roll_kling16"
    )

if __name__ == "__main__":
    main()
