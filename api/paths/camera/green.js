//
// Copyright 2018-2021 Orange
//
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
var mode="green";

module.exports = function(CameraService){
    let operations = {
      PUT,
      GET
    };
   
    async function PUT(req, res, next) {
        let now = new Date()
        console.log(now.toISOString()+" \t"+req.method+" \t"+req.originalUrl);
        await CameraService.updateCamera(mode);
        res.status(204).end();
    }
    async function GET(req, res, next) {
        let now = new Date()
        console.log(now.toISOString()+" \t"+req.method+" \t"+req.originalUrl)
        //try {
            color=true
            top=0
            left=0
            angle=0
            width=0
            height=0
            if (req.query.hasOwnProperty('color')) color=req.query.color
            if (req.query.hasOwnProperty('angle')) angle=req.query.angle
            if (req.query.hasOwnProperty('top')) top=req.query.top
            if (req.query.hasOwnProperty('left')) left=req.query.left
            if (req.query.hasOwnProperty('width')) width=req.query.width
            if (req.query.hasOwnProperty('height')) height=req.query.height
            var data=await CameraService.getCamera(mode,color,top,left,width,height,angle);
            res.set('Content-Type',"image/jpeg");
            res.status(200).end(data);
        /*} catch(error) {
            console.log("An error occurs on GET Photo " + JSON.stringify(error))
            res.status(500).json({
                'message': JSON.stringify(error)
            })
        }*/
    }
   
    // NOTE: We could also use a YAML string here.
    PUT.apiDoc = {
        operationId: 'updateCamera'+mode,
        summary: 'Update the photo for the camera with '+mode,
        security: [],
        responses: {
            '204': {
                description: 'The image is updated',
            }
        }
    };
    GET.apiDoc = {
        operationId: 'getLastCameraImage'+mode,
        summary: 'Retrieve the photo for the camera with '+mode,
        security: [],
        parameters: [{
            "name": "color",
            "in": "query",
            "description": "False to reurn greyscale image",
            "required": false,
            "schema": {
              "type": "boolean"
            }
        },{
            "name": "angle",
            "in": "query",
            "description": "Rotate the image",
            "required": false,
            "schema": {
                "type": "number"
            }
        },{
            "name": "top",
            "in": "query",
            "description": "Top corner for extraction, default 0",
            "required": false,
            "schema": {
                "type": "number"
            }
        },{
            "name": "left",
            "in": "query",
            "description": "Left corner for extraction, default 0",
            "required": false,
            "schema": {
                "type": "number"
            }
        },{
            "name": "width",
            "in": "query",
            "description": "width of the rectangle to extract, default 0 (no extraction)",
            "required": false,
            "schema": {
                "type": "number"
            }
        },{
            "name": "height",
            "in": "query",
            "description": "height of the rectangle to extract, default 0 (no extraction)",
            "required": false,
            "schema": {
                "type": "number"
            }
        }
        ],
        responses: {
            '200': {
                description: 'The Photo',
                content: {
                    'image/jpg': {
                        schema: {
                            type: 'string',
                            format: 'binary'
                        },
                    }
                }
            },
            '500': {
                description: 'An error occurs',
                content: {
                    'application/json': {
                        schema: {
                            $ref: '#/components/schemas/Error'
                        }
                    }
                }
            }
        }
    };
    
    return operations;
  }