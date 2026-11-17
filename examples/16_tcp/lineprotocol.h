#pragma once
#include <QByteArray>
#include <QList>

// 无网络依赖的协议层：只负责拆帧，便于测试拆包、粘包与长度限制。
class LineProtocol {
public:
    static constexpr int MaxPayload = 1024;
    bool feed(const QByteArray &bytes, QList<QByteArray> &lines) {
        // 调用者按有界块读取，避免一次缓存任意大小的数据。
        for (char byte : bytes) {
            if (byte == '\n') {
                lines.append(buffer_);
                buffer_.clear();
            } else {
                if (buffer_.size() >= MaxPayload) { buffer_.clear(); return false; }
                buffer_.append(byte);
            }
        }
        return true;
    }
private:
    QByteArray buffer_;
};
