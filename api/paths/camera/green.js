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
            if (req.query.hasOwnProperty('color')) color=req.query.color
            var data=await CameraService.getCamera(mode,color);
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