#include <QGuiApplication>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVulkanFunctions>
#include <QTimer>
#include <QDebug>

class ClearRenderer final : public QVulkanWindowRenderer {
public:
    explicit ClearRenderer(QVulkanWindow *window, int *frames) : window_(window), frames_(frames) {}
    void initResources() override {
        functions_ = window_->vulkanInstance()->deviceFunctions(window_->device());
    }
    void startNextFrame() override {
        VkClearValue clear[2] = {};
        clear[0].color.float32[0] = 0.10f;
        clear[0].color.float32[1] = 0.32f;
        clear[0].color.float32[2] = 0.48f;
        clear[0].color.float32[3] = 1.0f;
        clear[1].depthStencil = {1.0f, 0};
        VkRenderPassBeginInfo begin = {};
        begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin.renderPass = window_->defaultRenderPass();
        begin.framebuffer = window_->currentFramebuffer();
        begin.renderArea.extent.width = uint32_t(window_->swapChainImageSize().width());
        begin.renderArea.extent.height = uint32_t(window_->swapChainImageSize().height());
        begin.clearValueCount = 2;  // 本例固定单采样：颜色 + 深度/模板。
        begin.pClearValues = clear;
        const auto command = window_->currentCommandBuffer();
        functions_->vkCmdBeginRenderPass(command, &begin, VK_SUBPASS_CONTENTS_INLINE);
        functions_->vkCmdEndRenderPass(command);
        ++*frames_;
        window_->frameReady();
        // 静态清屏只需一帧；动画可在这里 requestUpdate()。
    }
private:
    QVulkanWindow *window_;
    QVulkanDeviceFunctions *functions_ = nullptr;
    int *frames_;
};
class ClearWindow final : public QVulkanWindow {
public:
    explicit ClearWindow(int *frames) : frames_(frames) {}
    QVulkanWindowRenderer *createRenderer() override { return new ClearRenderer(this, frames_); }
private:
    int *frames_;
};
int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QVulkanInstance instance;
    instance.setApiVersion(QVersionNumber(1, 0, 0));
    if (app.arguments().contains("--validation")) {
        if (!instance.supportedLayers().contains("VK_LAYER_KHRONOS_validation")) {
            qCritical() << "Requested validation layer is unavailable"; return 2;
        }
        instance.setLayers({"VK_LAYER_KHRONOS_validation"});
    }
    if (!instance.create()) { qCritical() << "Vulkan instance failed:" << instance.errorCode(); return 1; }
    int frames = 0;
    ClearWindow window(&frames); // 先销毁窗口，再销毁 instance 和帧计数。
    window.setVulkanInstance(&instance);
    window.setSampleCount(1);
    window.setTitle(QStringLiteral("示例 18 · Vulkan 清屏"));
    window.resize(640,480); window.show();
    if (app.arguments().contains("--self-test")) {
        QTimer::singleShot(2000, &window, [&] {
            if (!window.isValid() || frames == 0) { qCritical() << "No Vulkan frame recorded"; app.exit(3); }
            else { qInfo() << "PASS Vulkan instance, device, swapchain and command recording"; app.exit(0); }
        });
    }
    return app.exec();
}
