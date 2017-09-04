/***************************************************************
  **************javascript主要用来写函数算法来供qml使用**************
  **************************************************************/

var maxItem = 300                                  //就家居中的最大设备数目
var board = new Array(maxItem)        //一个模板数组,一个界面最多放150个设备
var component                                       //容器

/***************动态创建设备点********************/
function addDevItem(xPos , yPos , id ,type){
    if (component == null)
        component = Qt.createComponent("Position.qml");
    if (component.status == Component.Ready) {
        var dynamicObject = component.createObject(roomMap.contentItem);
        if (dynamicObject == null) {
            console.log("error creating block");
            console.log(component.errorString());
            return false;
        }
    }
    dynamicObject.x = xPos-11
    dynamicObject.y = yPos-22
    dynamicObject.id = id
    dynamicObject.type = type
    dynamicObject.opacity = 0
    board[roomMap.num++] = dynamicObject
}

/******************删除房间内所有的设备点***************/
function removeAll(){
    var count = 0
    for(count = 0;count<roomMap.num;count++){
            board[count].destroy()//.opacity = 0
        board[count] = null
    }
    roomMap.num = 0
    board = new Array(maxItem)
}

/**************显示房间内的设备点*********************/
function showDevIcon(type,id){
    var count = 0
    for(count = 0;count<roomMap.num;count++){
        if(board[count].id==id && board[count].type == type)
            board[count].opacity = 1
    }
}

/*******************隐藏某个id的设备*****************/
function hideDevIcon(type,id){
    var count = 0
    for(count = 0;count<roomMap.num;count++){
        if(board[count].id==id && board[count].type == type)
            board[count].opacity = 0
    }
}

/***************************************************************************************************************
  函数名：setDevInfo
  说明：修改设备的信息，地址和类型
  参数：
    oldDevId:初始的设备ＩＤ
    oldDevType:初始的设备类型
    newDevId:新的设备ＩＤ
    newDevType:新的设备类型
 ****************************************************************************************************************/
function setDevInfo(oldDevId,oldDevType,newDevId,newDevType){
    var count = 0
    for(count = 0;count<roomMap.num;count++){
        if(board[count].id==oldDevId && board[count].type == oldDevType)
        {
            board[count].id = newDevId
            board[count].type = newDevType
            break
        }
    }
}

