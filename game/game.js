var requestAnimFrame = (function(){
    return window.requestAnimationFrame       ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame    ||
        window.oRequestAnimationFrame      ||
        window.msRequestAnimationFrame     ||
        function(callback){
            window.setTimeout(callback, 1000 / 60);
        };
})();

var canvas=document.getElementById("gameCanvas");
var ctx=canvas.getContext("2d");
var cards = [];
var displayedCards = [];
var distinguishedCards = [];
var shirtImg = new Image();
var score = 0;
var initialDisplayedCardsNum = 12;
var secsLeft = 120;
shirtImg.src = 'shirt.jpg';

const canvasW = canvas.width;
const canvasH = canvas.height;

class Card{
    static get typesNum() {
        return 3;
    }

    static get charNum() {
        return 4;
    }

    static get numbers(){
        return [1, 2, 3];
    }

    static get shapes(){
        return ["ellipse", "rect", "wave"];
    }

    static get colors(){
        return ["red", "green", "purple"];
    }

    static get fillings(){
        return ["empty", "shaded", "filled"];
    }

    static get width() {
        return 70;
    }

    static get height(){
        return 100;
    }

    distinguish(){
        if (distinguishedCards.length >= Card.typesNum) return;

        this.distinguished = true;
        distinguishedCards.push(this);
    }

    undistinguish(){
        this.distinguished = false;
        distinguishedCards.splice(distinguishedCards.indexOf(this), 1);
    }


    constructor(number, shape, color, filling){
        this.number = number;
        this.shape = shape;
        this.color = color;
        this.filling = filling;
        this.distinguished = false;
        this.inverted=false;
        this.x = 0
        this.y = 0
    }

    display(x, y){
        this.x = x;
        this.y = y;

        if (cards.includes(this))
            cards.splice(cards.indexOf(this), 1);

        if (!displayedCards.includes(this))
            displayedCards.push(this);

        if(this.inverted){
            ctx.drawImage(shirtImg, x, y, Card.width, Card.height);
        }else {

            if(this.distinguished){
                ctx.lineWidth = 3;
                ctx.strokeStyle = ctx.fillStyle = "rgb(255, 0, 0)";
            }else{
                ctx.lineWidth = 1;
                ctx.strokeStyle = ctx.fillStyle = "rgb(0, 0, 0)";
            }


            ctx.beginPath();

            ctx.rect(x, y, Card.width, Card.height)
            ctx.stroke();
            ctx.closePath();

            ctx.lineWidth = 1;

            switch (this.color) {
                case "red":
                    ctx.strokeStyle = ctx.fillStyle = "rgb(255, 0, 0)";
                    break;
                case "green":
                    ctx.strokeStyle = ctx.fillStyle = "rgb(0, 255, 0)";
                    break;
                case "purple":
                    ctx.strokeStyle = ctx.fillStyle = "rgb(255, 0, 255)";
                    break;
            }

            const elW = Card.width / 2;
            const elH = Card.height / 8;
            var drawEl;
            switch (this.shape) {
                case "ellipse":
                    drawEl = function (_y) {
                        ctx.ellipse(x + Card.width / 2, y + _y + elH / 2, elW / 2, elH / 2, 0, 0, 2 * Math.PI);
                    }
                    break;
                case "rect":
                    drawEl = function (_y) {
                        ctx.rect(x + (Card.width - elW) / 2, y + _y, elW, elH);
                    }
                    break;
                case "wave":
                    drawEl = function (_y) {
                        ctx.moveTo(x + (Card.width - elW) / 2, y + _y + elH / 2);
                        ctx.quadraticCurveTo(x + Card.width / 2 + elW * 1 / 4, y + _y + elH, x + Card.width - elW / 2, y + _y);
                        ctx.moveTo(x + (Card.width - elW) / 2, y + _y + elH / 2);
                        ctx.quadraticCurveTo(x + Card.width / 2 - elW * 1 / 4, y + _y, x + Card.width - elW / 2, y + _y);
                    }
                    break;
            }

            var startY = (Card.height - this.number * (elH + 4)) / 2;
            for (let i = 0; i < this.number; i++) {
                ctx.beginPath();

                drawEl(startY + i * (elH + 4));
                ctx.stroke();


                switch (this.filling) {
                    case "shaded":
                        var my_gradient = ctx.createLinearGradient(x, y + startY + i * (elH + 4), x, y + startY + i * (elH + 4) + elH);
                        my_gradient.addColorStop(0, ctx.strokeStyle);
                        my_gradient.addColorStop(1, "black");
                        ctx.fillStyle = my_gradient;
                    case "filled":
                        ctx.fill();

                }

                ctx.closePath();
            }
        }
    }

    redisplay(){
        ctx.clearRect(this.x - 2, this.y - 2, Card.width + 4, Card.height + 4);
        this.display(this.x, this.y);
        ctx.flush();
    }
}

function shuffle(array) {
    let counter = array.length;

    while (counter > 0) {
        let index = Math.floor(Math.random() * counter);

        counter--;

        let temp = array[counter];
        array[counter] = array[index];
        array[index] = temp;
    }

    return array;
}

function generateCards(){

    let count = 0;
    for( let i = 0; i < Card.typesNum; i++ ){
        for( let j = 0; j < Card.typesNum; j++ ){
            for( let k = 0; k < Card.typesNum; k++ ){
                for( let t = 0; t < Card.typesNum; t++ ){
                    cards[count++] = new Card(Card.numbers[i], Card.shapes[j], Card.colors[k], Card.fillings[t]);
                }
            }
        }
    }

    shuffle(cards);
}

function displayTableSet(x, y, num) {
    t = Math.floor(Math.sqrt(num) * Card.height / Card.width);
    for( let i = 0; i < num; i++ ){
        cards[i].display(x + (Card.width + 3) * (i % t), y + (Card.height + 3) *  Math.floor(i / t));
    }
}

function updateTableSet(x, y) {

    for (let i = 0; i < distinguishedCards.length && cards; i++) {
        if(displayedCards.length <= initialDisplayedCardsNum){
            displayedCards[displayedCards.indexOf(distinguishedCards[i])] = cards.pop();
        }else{
            displayedCards.splice(displayedCards.indexOf(distinguishedCards[i]), 1);
        }
    }

    distinguishedCards = [];

    ctx.clearRect(0, 0, canvasW, canvasH);

    var t = Math.floor(Math.sqrt(displayedCards.length) * Card.height / Card.width);
    for(let i = 0; i < displayedCards.length; i++) {
        displayedCards[i].display(x + (Card.width + 3) * (i % t), y + (Card.height + 3) * Math.floor(i / t));
    }

    updateCardsLeftLabel();
}

function appendToTableSet(x, y, num) {
    score--;

    for(let i = 0; i < num; i++){
        if(cards){
            displayedCards.push(cards.pop());
        }
    }

    updateLabels();

    ctx.clearRect(0, 0, canvasW, canvasH);

    var t = Math.floor(Math.sqrt(displayedCards.length) * Card.height / Card.width);
    for(let i = 0; i < displayedCards.length; i++) {
        displayedCards[i].display(x + (Card.width + 3) * (i % t), y + (Card.height + 3) * Math.floor(i / t));
    }
}

function addCardsEventListeners() {
    canvas.onclick = function (e) {
        var x;
        var y;
        if (e.pageX || e.pageY) {
            x = e.pageX;
            y = e.pageY;
        }
        else {
            x = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
            y = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
        }
        x -= canvas.offsetLeft;
        y -= canvas.offsetTop;

        for (let i = 0; i < displayedCards.length; i++) {
            var card = displayedCards[i];

            if ((x >= card.x && x <= card.x + Card.width) && (y >= card.y && y <= card.y + Card.height)) {
                if (card.distinguished) {
                    card.undistinguish();
                } else {
                    card.distinguish();
                }

                if (isSetFound()) {
                    updateTableSet(1, 1);
                    score += 3;
                    updateScoreLabel();
                    return;
                }

                card.redisplay();
            }
        }
    }
}


function updateLabels() {
    updateScoreLabel();
    updateCardsLeftLabel();
    updateTimeLeftLabel();
}

function updateScoreLabel() {
    document.getElementById("scoreLabel").innerText = "score: " + score;
}

function updateCardsLeftLabel() {
    document.getElementById("cardsLeftLabel").innerText = "cards left: " + cards.length;
}

function updateTimeLeftLabel() {
    document.getElementById("timeLeftLabel").innerText = "seconds left: " + secsLeft;
}

function isSetFound() {
    if (distinguishedCards.length != Card.typesNum) return false;

    var checkArr = [];

    for( let j = 0; j < Card.charNum; j++ ) {
        checkArr[j] = {};
        checkArr[j]["diffCheck"] = true;
        checkArr[j]["sameCheck"] = true;
    }

    for( let i = 0; i < distinguishedCards.length; i++ ) {
        for( let j = i + 1; j < distinguishedCards.length; j++ ) {
            if (distinguishedCards[i].number != distinguishedCards[j].number){
                checkArr[0]["sameCheck"] = false;
            }else{
                checkArr[0]["diffCheck"] = false;
            }

            if (distinguishedCards[i].shape != distinguishedCards[j].shape){
                checkArr[1]["sameCheck"] = false;
            }else{
                checkArr[1]["diffCheck"] = false;
            }

            if (distinguishedCards[i].color != distinguishedCards[j].color){
                checkArr[2]["sameCheck"] = false;
            }else{
                checkArr[2]["diffCheck"] = false;
            }

            if (distinguishedCards[i].filling != distinguishedCards[j].filling){
                checkArr[3]["sameCheck"] = false;
            }else{
                checkArr[3]["diffCheck"] = false;
            }
        }
    }

    for ( let j = 0; j < Card.charNum; j++ ){
        if (!checkArr[j]["diffCheck"] && !checkArr[j]["sameCheck"]){
            return false;
        }
    }

    return true;
}

function startSetGame(){
    document.getElementById("addCardsButton").onclick = function(){ appendToTableSet(1, 1, 3); }
    generateCards();
    displayTableSet(1, 1, initialDisplayedCardsNum);
    addCardsEventListeners();
    updateLabels();
    setInterval(function () {
        secsLeft--;
        updateTimeLeftLabel();
        if(secsLeft <= 0){
            document.getElementById("SetGame").remove();
        }
    }, 1000);
}