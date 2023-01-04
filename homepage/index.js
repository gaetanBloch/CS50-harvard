// Usage of trivia API
// Creative Commons License
// https://the-trivia-api.com/license/
const TRIVIA_URI = "https://the-trivia-api.com/api/questions?limit=1&"

const includeHeader = () => {
  $('#header').load('header.html');
};

const includeFooter = () => {
  $('#footer').load('footer.html');
};

const includeHome = () => {
  $('#home').load('home.html');
};

const includeQuestion = () => {
  $('#question').load('question.html');
};

const includeSpinner = () => {
  $('#spinner').load('spinner.html');
};

const displaySpinner = () => {
  document.querySelector('#spinner').classList.remove('no-display');
  document.querySelector('#home').classList.add('no-display');
};

const displayQuestion = () => {
  document.querySelector('#spinner').classList.add('no-display');
  document.querySelector('#question').classList.remove('no-display');
};

const getUrlParams = () => {
  const queryString = window.location.search;
  return new URLSearchParams(queryString);
};

const buildParam = (urlParams, param) => {
  return `${param}=${urlParams.get(param)}&`
};

const buildQueryUri = (urlParams) => {
  return `${TRIVIA_URI}${buildParam('category')}${buildParam('difficulty')}${buildParam('region')}`
};

const getResponse = (response) => {
  return {
    question: response.question,
    correct: response.correctAnswer,
    incorrect: response.incorrectAnswers,
  }
};

const buildTrivia = (response) => {
  document.querySelector(".question").innerHTML = response.question;
  const answerIndex = Math.floor(Math.random() * 5);
  const correctButton = document.querySelector("#b" + answerIndex);
  correctButton.innerHTML = response.correct;
  correctButton.setAttribute('data-correct', true);
  [1, 2, 3, 4].forEach((i) => {
      if (i < answerIndex) {
        document.querySelector("#b" + i).innerHTML =
          response.incorrect[answerIndex - i - 1];
      } else if (i > answerIndex) {
        document.querySelector("#b" + i).innerHTML =
          response.incorrect[i - 1 - 1];
      }
  });

  document.querySelectorAll('.answer').forEach((el) => {
    el.addEventListener('click', (e) => {
      const result = document.querySelector('#result');
      if (e.target.dataset?.correct) {
        e.target.style.backgroundColor = 'var(--bs-green)';
        e.target.style.color = 'white';
        result.innerHTML = 'Correct! 👌 ';
      } else {
        e.target.style.backgroundColor = 'var(--bs-red)';
        e.target.style.color = 'white';
        result.innerHTML = 'Incorrect 🙅 ';
      }
      e.preventDefault();
    });
  });
};

const bootstrap = async () => {
  // Bootstrap program
  const urlParams = getUrlParams();
  if (urlParams.has('category')) {
    // A request was made
    displaySpinner();
    const responseRaw = await fetch(TRIVIA_URI);
    const response = getResponse((await responseRaw.json())[0]);
    setTimeout(() => {
      buildTrivia(response);
      displayQuestion();
    }, 1000);
  } else {
    console.log("Set up your trivia question");
  }
};