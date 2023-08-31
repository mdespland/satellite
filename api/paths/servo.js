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

module.exports = function(ServoService){
    let operations = {
      PUT
    };
   
    async function PUT(req, res, next) {
        let now = new Date()
        console.log(now.toISOString()+" \t"+req.method+" \t"+req.originalUrl);
        await ServoService.moveToAngle(req.query.angle);
        res.status(204).end();
    }

    // NOTE: We could also use a YAML string here.
    PUT.apiDoc = {
        operationId: 'moveServoToAngle',
        summary: 'Move the servo to the specified angle',
        security: [],
        parameters: [{
            "name": "angle",
            "in": "query",
            "description": "The angle to move the servo",
            "required": true,
            "schema": {
              "type": "number"
            }
        }
        ],
        responses: {
            '204': { 
                description: 'Operation done',
            }
        }
    };

    
    return operations;
  }