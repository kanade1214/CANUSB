# CANUSB 

created by kanade @kanade___1214

CAN通信とUSBのブリッジ基板です。  
高専ロボコン2023用に作成したものを配布用に安価に、誰でも使えるようにしました。  
作成、使用は自由ですが、kanadeに一声DMしていただけると喜びます。つながりを増やしたいので  

プログラムを書き込む前に”ファイル名”の””行目にてUSBのPIDをご自身のものに書き換えてください。  
~そのままで動く！わからない人はそのままで！~  


ROS2パッケージの説明を下に記述します。
(ROS2協力：@Pine_Pino_Pine)


# ROS2 can_communication

文責：Pine

## 前提

- ROS 2 Humble と Ubuntu 22.04 で動作確認済み
- Pythonライブラリpyserialを使用
    
    ```bash
    $ pip install pyserial
    ```
    
- カスタムメッセージ`CanMsg.msg`を使用
    
    ```cpp
    bool channel
    bool frametype
    uint32 id
    byte dlc
    byte[<=8] data
    ```
    
- kanade設計のCAN-USB基板を使用

## トピック

### publish

USB-CAN基板から受け取ったCANパケットをCanMsg型topic`/can_rx`としてpublishする

### subscribe

subscribeしたCanMsg型topic`/can_tx`をUSB-CAN基板にserialとして流す

## 通信仕様

### Serial

|  | 1 byte目 | 2 byte目 | 3 byte目 | 4~6 byte目 | 7 byte目 | 8~15 byte目 | 最後 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 目的 | メッセージ開始 | チャンネル | フレームタイプ | ID | DLC | データ | メッセージ終了 |
| 形式 | 0x02 | ASCIIコード | ASCIIコード | ASCIIコード | ASCIIコード |  | 0x03 |
| 備考 | tx時のみ　rxでは無し | ‘0’ or ‘1’ | ‘0’ or ‘1’ | 16進数 3桁 | ’1’ ~ ‘8’ | DLCに対応した長さ | tx時のみ　rxでは無し |

フレームタイプが1（リモートフレーム）のときはDLCに関わらずdataは無効（rx時は無視され、tx時は空）

### CanMsg

|  | 目的 | 型 (ROS) | 型 (Python) | 型 (C++) | 形式 |
| --- | --- | --- | --- | --- | --- |
| channel | 使用するCAN Busのチャンネル | bool | bool | bool | false: ch0　true: ch1 |
| frametype | データフレームとリモートフレームの選択 | bool | bool | bool | false: データフレーム　true: リモートフレーム |
| id | CANデバイスのID | uint32 | int | uint32_t | 非負整数（例：0x201なら513） |
| dlc | データのbyte長 | byte | bytes | uint8_t | ASCIIコード 1文字 |
| data | データ | byte配列 | bytesリスト | uint8_t配列 | 各デバイスの仕様による |

## Pythonでの使用例

### TX

```python
import rclpy
from rclpy.node import Node
from custom_messages.msg import CanMsg

class publisher(Node) :
    def __init__(self) :
        super().__init__('publisher')
        
        self.pub = self.create_publisher(CanMsg, '/can_tx', 1)
        
        # 1秒周期で整数値とテキストを送信させる
        self.create_timer(1, self.publish_integer)
        self.create_timer(1, self.publish_text)
    
    def publish_integer(self) :
        # 送信する数値（符号なし8bit整数型の範囲内）
        num = 120
        
        msg = CanMsg()
        msg.channel = False
        msg.frametype = False
        msg.id = 0x101
        msg.dlc = b'1'                  # int型でもstr型でもなくbytes型
        msg.data = [str(num).encode()]  # 要素が一つでもリストにする
        
        self.pub.publish(msg)
        
    def publish_text(self) :
        # 送信するテキスト（str型ではなくbytes型）
        txt = b'Hello'
        
        msg = CanMsg()
        msg.channel = False
        msg.frametype = False
        msg.id = 0x102
        msg.dlc = str(len(txt)).encode()      # DLCを自動設定
        msg.data = [bytes([c]) for c in txt]  # これが少し複雑なので注意
        
        self.pub.publish(msg)

def main() :
    rclpy.init()
    node = publisher()
    
    try :
        rclpy.spin(node)
    except Exception :
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__' :
    main()
```

### RX

```python
import rclpy
from rclpy.node import Node
from custom_messages.msg import CanMsg

class subscriber(Node) :
    def __init__(self) :
        super().__init__('subscriber')
        
        self.data1 = 0
        self.data2 = 0
        
        self.sub = self.create_subscription(CanMsg, '/can_rx', self.callback, 10)
    
    def callback(self, msg) :
　　　　　# idによって処理を分岐
        if msg.id == 0x201 :
            # 整数値が1byteで送信される場合
            self.data1 = int.from_bytes(msg.data[0], 'big')
        elif msg.id == 0x202 :
            # 整数値がASCIIコード列として送信される場合
            self.data2 = int(b''.join(msg.data))

def main() :
    rclpy.init()
    node = subscriber()
    
    try :
        rclpy.spin(node)
    except Exception :
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__' :
    main()
```
