#!/usr/bin/env python3
# coding=utf-8

import os
import sys
import requests
import tarfile
import shutil

def download_toolchain(toolchain_root):
    toolchain_path = os.path.join(toolchain_root, "gcc-arm-none-eabi-10-2020-q4-major")
    if os.path.exists(toolchain_path):
        print("Toolchain already exists.")
        return True

    url = "https://airtake-public-data-1254153901.cos.ap-shanghai.myqcloud.com/smart/embed/pruduct/l511a_0.0.1.zip"
    save_path = os.path.join(toolchain_root, "gcc-arm-none-eabi-10-2020-q4-major.gz")
    temp_extract_path = os.path.join(toolchain_root, "temp_extract")

    try:
        # 发送 GET 请求
        # print("开始下载工具链...")
        response = requests.get(url, stream=True)
        # 检查请求是否成功
        response.raise_for_status()
        
        # 打开文件准备写入
        with open(save_path, 'wb') as file:
            # 写入文件
            for chunk in response.iter_content(chunk_size=8192):
                if chunk:
                    file.write(chunk)
        # print(f"文件已成功下载到: {save_path}")

        # 如果临时目录存在，先删除
        if os.path.exists(temp_extract_path):
            shutil.rmtree(temp_extract_path)
        
        # 创建临时解压目录
        os.makedirs(temp_extract_path, exist_ok=True)

        # 打开tar.gz文件并解压到临时目录
        with tarfile.open(save_path, 'r:gz') as tar_ref:
            tar_ref.extractall(temp_extract_path)
        # print(f"文件已解压到: {temp_extract_path}")

        # 显示解压后的实际目录结构
        # print("解压后的目录结构:")
        for root, dirs, files in os.walk(temp_extract_path):
            level = root.replace(temp_extract_path, '').count(os.sep)
            indent = ' ' * 2 * level
            print(f"{indent}{os.path.basename(root)}/")
            subindent = ' ' * 2 * (level + 1)
            for file in files:
                print(f"{subindent}{file}")
        
        # 查找 gcc-arm-none-eabi-10-2020-q4-major 目录
        extracted_toolchain_path = None
        for root, dirs, files in os.walk(temp_extract_path):
            for dir_name in dirs:
                if dir_name == "gcc-arm-none-eabi-10-2020-q4-major":
                    extracted_toolchain_path = os.path.join(root, dir_name)
                    break
            if extracted_toolchain_path:
                break
        
        if extracted_toolchain_path and os.path.exists(extracted_toolchain_path):
            # 如果目标位置已存在，先删除
            if os.path.exists(toolchain_path):
                shutil.rmtree(toolchain_path)
            
            # 移动目录到正确位置
            shutil.move(extracted_toolchain_path, toolchain_path)
            # print(f"已将工具链移动到: {toolchain_path}")
        else:
            raise Exception("未找到 gcc-arm-none-eabi-10-2020-q4-major 目录")

        # 清理临时目录和压缩包
        shutil.rmtree(temp_extract_path)
        os.remove(save_path)
        # print("临时文件已清理")
        
    except requests.exceptions.RequestException as e:
        print(f"下载失败: {e}")
        return False
    except Exception as e:
        print(f"处理失败: {e}")
        # 清理可能存在的临时文件
        if os.path.exists(temp_extract_path):
            shutil.rmtree(temp_extract_path)
        if os.path.exists(save_path):
            os.remove(save_path)
        return False

    return True

def main():
    root = os.path.dirname(os.path.abspath(__file__))
    platform_root = os.path.abspath(os.path.join(root, ".."))
    toolchain_root = os.path.join(platform_root, "tools")
    os.makedirs(toolchain_root, exist_ok=True)
    print(f"platform_root: {toolchain_root}")

    if not download_toolchain(toolchain_root):
        print("Error: download toolchain failed.")
        sys.exit(1)

    sys.exit(0)


if __name__ == "__main__":
    main()

