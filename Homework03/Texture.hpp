//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H

#include "global.hpp"
//#include <eigen3/Eigen/Eigen>
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture
{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }
    
    /// <summary>
    /// 双线性纹理插值
    /// </summary>
    /// <param name="u"></param>
    /// <param name="v"></param>
    /// <returns></returns>
    Eigen::Vector3f getBilinearColor2(float u, float v)
    {
        float u_img = u * width;
        float v_img = (1 - v) * height;

        int x = (int)floor(u_img);
        int y = (int)floor(v_img);        
        int x1 = std::min((int)ceil(u_img), width);
        int y1 = std::min((int)ceil(v_img), height);

        float s = u_img - x;
        float t = v_img - y;

        auto c00 = image_data.at<cv::Vec3b>(y1, x);  // color1
        auto c10 = image_data.at<cv::Vec3b>(y1, x1); // color2
        auto c01 = image_data.at<cv::Vec3b>(y, x);   // color3
        auto c11 = image_data.at<cv::Vec3b>(y, x1);  // color4

        auto r_u = s / (x1 - x);
        auto r_v = s / (y1 - y);

        auto up = (1- r_u) * c01  + r_u * c11;
        auto down = (1 - r_u) * c00 + r_u * c10;
        auto color = (1 - r_v) * up + r_v * down;

        return Eigen::Vector3f(color[0], color[1], color[2]);
    } 

    Eigen::Vector3f getBilinearColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;

        auto x = (int)floor(u_img);
        auto x1 = std::min((int)ceil(u_img), width);
        auto y = (int)floor(v_img);
        auto y1 = std::min((int)ceil(v_img), height);

        auto color1 = image_data.at<cv::Vec3b>(y1, x);
        auto color2 = image_data.at<cv::Vec3b>(y1, x1);
        auto color3 = image_data.at<cv::Vec3b>(y, x);
        auto color4 = image_data.at<cv::Vec3b>(y, x1);

        auto ratio_u = (u_img - x) / (x1 - x);
        auto ratio_v = (v_img - y) / (y1 - y);
        auto up = (1 - ratio_u) * color3 + ratio_u * color4;
        auto down = (1 - ratio_u) * color1 + ratio_u * color2;
        auto col = (1 - ratio_v) * up + ratio_v * down;

        return Eigen::Vector3f(col[0], col[1], col[2]);
    }
};
#endif //RASTERIZER_TEXTURE_H
