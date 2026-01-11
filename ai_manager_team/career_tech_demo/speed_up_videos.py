# -*- coding: utf-8 -*-
"""
MP4 파일들을 4배속으로 변환하는 스크립트
"""

import subprocess
import os
from pathlib import Path

def speed_up_video(input_path: Path, output_path: Path, speed: float):
    """FFmpeg를 사용하여 영상 속도를 변경합니다."""
    
    # 비디오: setpts=PTS/speed
    video_filter = f"setpts=PTS/{speed}"
    
    # atempo는 0.5~2.0 범위만 지원하므로 4배속은 체이닝 필요
    # 4배속: atempo=2.0 * atempo=2.0 = 4.0
    if speed <= 2.0:
        audio_filter = f"atempo={speed}"
    elif speed <= 4.0:
        audio_filter = "atempo=2.0,atempo=2.0"
    else:
        audio_filter = "atempo=2.0,atempo=2.0,atempo=2.0"
    
    cmd = [
        "ffmpeg",
        "-i", str(input_path),
        "-filter:v", video_filter,
        "-filter:a", audio_filter,
        "-y",  # 덮어쓰기
        str(output_path)
    ]
    
    print(f"변환 중: {input_path.name} ({speed}배속)")
    result = subprocess.run(cmd, capture_output=True)
    
    if result.returncode != 0:
        print(f"  오류 발생")
        return False
    
    # 파일 크기 비교
    original_size = input_path.stat().st_size / (1024 * 1024)
    new_size = output_path.stat().st_size / (1024 * 1024)
    print(f"  완료: {original_size:.1f}MB -> {new_size:.1f}MB")
    return True


def main():
    script_dir = Path(__file__).parent
    output_dir = script_dir / "fast"
    output_dir.mkdir(exist_ok=True)
    
    mp4_files = list(script_dir.glob("*.mp4"))
    
    if not mp4_files:
        print("MP4 파일을 찾을 수 없습니다.")
        return
    
    print(f"발견된 MP4 파일: {len(mp4_files)}개")
    print(f"출력 폴더: {output_dir}")
    print("-" * 50)
    
    speed = 4.0  # 모두 4배속
    
    for mp4_file in mp4_files:
        output_path = output_dir / mp4_file.name  # 파일명 유지
        speed_up_video(mp4_file, output_path, speed)
    
    print("-" * 50)
    print("완료!")


if __name__ == "__main__":
    main()
