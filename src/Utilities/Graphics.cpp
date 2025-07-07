
#include "Graphics.hpp"



void Graphics::SetupFramebuffer(Framebuffer* fb, glm::ivec2 size, Shader *shader) {
    fb->size = size;
    fb->shader = shader;
    BindFrameBuffer(fb);
    fb->shader->use();
    glUniform1i(glGetUniformLocation(shader->ID, "renderTexture"), 0); // GL_TEXTURE0
}

void Graphics::SetupFramebuffer(RenderFramebuffer* fb, glm::ivec2 size, Shader *shader) {
    fb->size = size;
    fb->shader = shader;
    BindFrameBuffer(fb);
    fb->shader->use();
    glUniform1i(glGetUniformLocation(shader->ID, "renderTexture"), 0); // GL_TEXTURE0
    glUniform1i(glGetUniformLocation(shader->ID, "depthTexture"), 1); // GL_TEXTURE1
}


void Graphics::BindFrameBuffer(Framebuffer *fb){

    if (fb->fbo == 0) glGenFramebuffers(1, &fb->fbo);
    if (fb->renderTexture == 0) glGenTextures(1, &fb->renderTexture);


    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);

    glBindTexture(GL_TEXTURE_2D, fb->renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->size.x, fb->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->renderTexture, 0);  

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}
void Graphics::BindFrameBuffer(RenderFramebuffer* fb) {

    if (fb->fbo == 0) glGenFramebuffers(1, &fb->fbo);
    if (fb->renderTexture == 0) glGenTextures(1, &fb->renderTexture);
    if (fb->depthTexture == 0) glGenTextures(1, &fb->depthTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);

    glBindTexture(GL_TEXTURE_2D, fb->renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->size.x, fb->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glBindTexture(GL_TEXTURE_2D, fb->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fb->size.x, fb->size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->renderTexture, 0);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb->depthTexture, 0);  
    
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}



void Graphics::ChangeFramebufferSize(Framebuffer *fb, glm::ivec2 size) {
    fb->size = size;
    BindFrameBuffer(fb);
}
void Graphics::ChangeFramebufferSize(RenderFramebuffer *fb, glm::ivec2 size) {
    fb->size = size;
    BindFrameBuffer(fb);
}

void Graphics::DrawFrame(RenderFramebuffer *fb, unsigned int VAO) {
    fb->shader->use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb->renderTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fb->depthTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);  
}

void Graphics::DrawFrame(Framebuffer *fb, unsigned int VAO) {
    fb->shader->use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb->renderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);  
}
