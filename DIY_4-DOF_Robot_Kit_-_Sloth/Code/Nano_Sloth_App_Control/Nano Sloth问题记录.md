# Nano Sloth问题记录

- [] arduniojson 对乱码会处理成 False， 实际需要做跳过处理
  
- [] 解决乱码问题
    - [] 长度校验
    - [] 缩短读取间隔
        - ws_loop 16ms ~ 991ms, (esp01s, 100ms 发送一次）
        - 基本是舵机控制占时间

- [] 尝试细分动作的执行