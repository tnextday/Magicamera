//
//  Shader.fsh
//  MagiCamera
//
//  Created by Feng Huo on 12-2-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
